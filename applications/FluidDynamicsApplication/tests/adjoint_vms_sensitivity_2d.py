import os
from KratosMultiphysics import *
import KratosMultiphysics.KratosUnittest as KratosUnittest
import KratosMultiphysics.FluidDynamicsApplication as KratosCFD
import KratosMultiphysics.kratos_utilities as kratos_utils

from  KratosMultiphysics.kratos_utilities import CheckIfApplicationsAvailable
missing_applications_message = ["Missing required application(s):",]
have_required_applications = CheckIfApplicationsAvailable("HDF5Application")
if have_required_applications:
    import KratosMultiphysics.HDF5Application as kh5
else:
    missing_applications_message.append("HDF5Application")

from KratosMultiphysics.FluidDynamicsApplication.fluid_dynamics_analysis import FluidDynamicsAnalysis
from KratosMultiphysics.FluidDynamicsApplication.adjoint_fluid_analysis import AdjointFluidAnalysis

class ControlledExecutionScope:
    def __init__(self, scope):
        self.currentPath = os.getcwd()
        self.scope = scope

    def __enter__(self):
        os.chdir(self.scope)

    def __exit__(self, type, value, traceback):
        os.chdir(self.currentPath)

@KratosUnittest.skipUnless(have_required_applications," ".join(missing_applications_message))
class AdjointVMSSensitivity2D(KratosUnittest.TestCase):

    def setUp(self):
        pass

    def _removeH5Files(self, model_part_name):
        for name in os.listdir():
            if name.find(model_part_name) == 0:
                kratos_utils.DeleteFileIfExisting(name)

    @staticmethod
    def _writeNodalCoordinates(lines, node_block_start, node_lines, node_id,
                               coords, model_part_file_name):
        old_line = node_lines[node_id]
        components = old_line.split()
        if int(components[0]) != node_id:
            raise RuntimeError('Error parsing file ' + model_part_file_name)
        new_line = '{:5d}'.format(node_id) + ' ' \
             + '{:19.10f}'.format(coords[0]) + ' ' \
             + '{:19.10f}'.format(coords[1]) + ' ' \
             + '{:19.10f}'.format(coords[2]) + '\n'
        lines[node_block_start + node_id] = new_line
        with open(model_part_file_name + '.mdpa', 'w') as model_part_file:
            model_part_file.writelines(lines)

    def _computeFiniteDifferenceSensitivity(self, node_ids, step_size,
                                            model_part_file_name,
                                            project_parameters_file_name,
                                            objective_function):
        with open(model_part_file_name + '.mdpa', 'r') as file_input:
            lines = file_input.readlines()

        node_block_start = lines.index('Begin Nodes\n')
        node_block_end = lines.index('End Nodes\n')
        node_lines = lines[node_block_start:node_block_end]

        sensitivity = []
        for node_id in node_ids:
            node_sensitivity = []
            coord = [
                float(node_lines[node_id].split()[i]) for i in range(1, 4)
            ]

            for k in range(2):
                coord[k] += step_size
                AdjointVMSSensitivity2D._writeNodalCoordinates(
                    list(lines), node_block_start, node_lines, node_id, coord,
                    model_part_file_name)
                node_sensitivity.append(objective_function(self.solve(project_parameters_file_name)))
                coord[k] -= step_size

            sensitivity.append(node_sensitivity)

        # now compute the unperturbed
        with open(model_part_file_name + '.mdpa', 'w') as model_part_file:
            model_part_file.writelines(lines)
        ref_value = objective_function(self.solve(project_parameters_file_name))

        # update finite difference sensitivities to correct values
        for node_sensitivity in sensitivity:
            for i, value in enumerate(node_sensitivity):
                node_sensitivity[i] = (value - ref_value) / step_size

        return sensitivity

    def _computeFiniteDifferenceDragSensitivity(self, node_ids, step_size,
                                                model_part_file_name,
                                                project_parameters_file_name,
                                                drag_direction,
                                                drag_file_name):
        def objective_function(_):
            return _getTimeAveragedDrag(drag_direction, drag_file_name)

        return self._computeFiniteDifferenceSensitivity(
            node_ids, step_size, model_part_file_name,
            project_parameters_file_name, objective_function)

    def _computeFiniteDifferenceNormSquareSensitivity(
        self, node_ids, step_size, model_part_file_name,
        project_parameters_file_name, response_value_output_file_name):

        def objective_function(_):
            with open(response_value_output_file_name, "r") as file_input:
                lines = file_input.readlines()

            value = 0.0
            total_time = 0.0
            delta_time = 0.0
            for line in lines[2:]:
                data = line.strip().split(",")
                delta_time = float(data[0]) - total_time
                total_time = float(data[0])
                value += float(data[1])

            return value * delta_time

        return self._computeFiniteDifferenceSensitivity(
            node_ids, step_size, model_part_file_name,
            project_parameters_file_name, objective_function)

    def _readParameters(self, parameter_file_name):
        with open(parameter_file_name + '_parameters.json', 'r') as parameter_file:
            project_parameters = Parameters(parameter_file.read())
            parameter_file.close()
        return project_parameters

    def _createFluidTest(self, parameter_file_name):
        test = FluidDynamicsAnalysis(Model(), self._readParameters(parameter_file_name))
        return test

    def _createAdjointTest(self, parameter_file_name):
        test = AdjointFluidAnalysis(Model(), self._readParameters(parameter_file_name))
        return test

    def solve(self, parameter_file_name):
        test = self._createFluidTest(parameter_file_name)
        test.Run()
        return test

    def testOneElement(self):
        with ControlledExecutionScope(os.path.dirname(os.path.realpath(__file__))):
            # calculate sensitivity by finite difference
            step_size = 0.00000001
            FDSensitivity = self._computeFiniteDifferenceDragSensitivity(
                [1], step_size,
                './AdjointVMSSensitivity2DTest/one_element_test',
                './AdjointVMSSensitivity2DTest/one_element_test',
                [1.0, 0.0, 0.0], './MainModelPart.Structure_drag.dat')

            # solve adjoint
            test = AdjointFluidAnalysis(Model(), self._readParameters('AdjointVMSSensitivity2DTest/one_element_test_adjoint'))
            test.Run()
            Sensitivity = [[]]
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1).GetSolutionStepValue(SHAPE_SENSITIVITY_X))
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1).GetSolutionStepValue(SHAPE_SENSITIVITY_Y))

            self.assertAlmostEqual(Sensitivity[0][0], FDSensitivity[0][0], 4)
            self.assertAlmostEqual(Sensitivity[0][1], FDSensitivity[0][1], 4)
            self._removeH5Files("MainModelPart")
            kratos_utils.DeleteFileIfExisting("./AdjointVMSSensitivity2DTest/one_element_test.time")
            kratos_utils.DeleteFileIfExisting("./Structure_drag.dat")
            kratos_utils.DeleteFileIfExisting("./one_element.post.bin")
            kratos_utils.DeleteFileIfExisting("./tests.post.lst")

    def testCylinder(self):
        with ControlledExecutionScope(os.path.dirname(os.path.realpath(__file__))):
            # calculate sensitivity by finite difference
            step_size = 0.00000001
            FDSensitivity = self._computeFiniteDifferenceDragSensitivity(
                [1968], step_size,
                './AdjointVMSSensitivity2DTest/cylinder_test',
                './AdjointVMSSensitivity2DTest/cylinder_test', [1.0, 0.0, 0.0],
                'MainModelPart.NoSlip2D_Cylinder_drag.dat')

            # solve adjoint
            test = self._createAdjointTest('AdjointVMSSensitivity2DTest/cylinder_test_adjoint')
            test.Run()
            Sensitivity = [[]]
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1968).GetSolutionStepValue(SHAPE_SENSITIVITY_X))
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1968).GetSolutionStepValue(SHAPE_SENSITIVITY_Y))

            self.assertAlmostEqual(Sensitivity[0][0], FDSensitivity[0][0], 5)
            self.assertAlmostEqual(Sensitivity[0][1], FDSensitivity[0][1], 5)
            self._removeH5Files("MainModelPart")
            kratos_utils.DeleteFileIfExisting("./AdjointVMSSensitivity2DTest/cylinder_test.time")
            kratos_utils.DeleteFileIfExisting("./NoSlip2D_Cylinder_drag.dat")
            kratos_utils.DeleteFileIfExisting("./cylinder_test.post.bin")
            kratos_utils.DeleteFileIfExisting("./tests.post.lst")

    def testSteadyCylinder(self):
        with ControlledExecutionScope(os.path.dirname(os.path.realpath(__file__))):
            # calculate sensitivity by finite difference
            step_size = 0.00000001
            FDSensitivity = self._computeFiniteDifferenceDragSensitivity(
                [1968], step_size,
                './AdjointVMSSensitivity2DTest/steady_cylinder_test',
                './AdjointVMSSensitivity2DTest/steady_cylinder_test',
                [1.0, 0.0, 0.0], 'MainModelPart.NoSlip2D_Cylinder_drag.dat')

            # solve adjoint
            test = self._createAdjointTest('AdjointVMSSensitivity2DTest/steady_cylinder_test_adjoint')
            test.Run()
            Sensitivity = [[]]
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1968).GetSolutionStepValue(SHAPE_SENSITIVITY_X))
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1968).GetSolutionStepValue(SHAPE_SENSITIVITY_Y))

            self.assertAlmostEqual(Sensitivity[0][0], FDSensitivity[0][0], 4)
            self.assertAlmostEqual(Sensitivity[0][1], FDSensitivity[0][1], 2)
            self._removeH5Files("MainModelPart")
            kratos_utils.DeleteFileIfExisting("./AdjointVMSSensitivity2DTest/steady_cylinder_test.time")
            kratos_utils.DeleteFileIfExisting("./NoSlip2D_Cylinder_drag.dat")
            kratos_utils.DeleteFileIfExisting("./steady_cylinder_test.post.bin")
            kratos_utils.DeleteFileIfExisting("./tests.post.lst")

    def testSlipNormCylinder(self):
        with ControlledExecutionScope(os.path.dirname(os.path.realpath(__file__))):
            # calculate sensitivity by finite difference
            step_size = 0.00000001
            FDSensitivity = self._computeFiniteDifferenceNormSquareSensitivity(
                [1968], step_size,
                './AdjointVMSSensitivity2DTest/cylinder_test',
                './AdjointVMSSensitivity2DTest/cylinder_slip_test',
                'velocity_pressure_norm_square_response_output.dat')

            # solve adjoint
            test = self._createAdjointTest('AdjointVMSSensitivity2DTest/cylinder_slip_test_adjoint')
            test.Run()
            Sensitivity = [[]]
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1968).GetSolutionStepValue(SHAPE_SENSITIVITY_X))
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1968).GetSolutionStepValue(SHAPE_SENSITIVITY_Y))

            self.assertAlmostEqual(Sensitivity[0][0], FDSensitivity[0][0], 9)
            self.assertAlmostEqual(Sensitivity[0][1], FDSensitivity[0][1], 9)
            self._removeH5Files("MainModelPart")
            kratos_utils.DeleteFileIfExisting("./AdjointVMSSensitivity2DTest/cylinder_test.time")
            kratos_utils.DeleteFileIfExisting("./velocity_pressure_norm_square_response_output.dat")
            kratos_utils.DeleteFileIfExisting("./cylinder_test.post.bin")
            kratos_utils.DeleteFileIfExisting("./tests.post.lst")

    def testSlipSteadyNormCylinder(self):
        with ControlledExecutionScope(os.path.dirname(os.path.realpath(__file__))):
            # calculate sensitivity by finite difference
            step_size = 1e-9
            FDSensitivity = self._computeFiniteDifferenceNormSquareSensitivity(
                [1968], step_size,
                './AdjointVMSSensitivity2DTest/steady_cylinder_test',
                './AdjointVMSSensitivity2DTest/steady_cylinder_slip_test',
                'velocity_pressure_norm_square_response_output.dat')

            # solve adjoint
            test = self._createAdjointTest('AdjointVMSSensitivity2DTest/steady_cylinder_slip_test_norm_adjoint')
            test.Run()
            Sensitivity = [[]]
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1968).GetSolutionStepValue(SHAPE_SENSITIVITY_X))
            Sensitivity[0].append(test._GetSolver().main_model_part.GetNode(1968).GetSolutionStepValue(SHAPE_SENSITIVITY_Y))

            self.assertAlmostEqual(Sensitivity[0][0], FDSensitivity[0][0], 7)
            self.assertAlmostEqual(Sensitivity[0][1], FDSensitivity[0][1], 6)
            self._removeH5Files("MainModelPart")
            kratos_utils.DeleteFileIfExisting("./AdjointVMSSensitivity2DTest/steady_cylinder_test.time")
            kratos_utils.DeleteFileIfExisting("./velocity_pressure_norm_square_response_output.dat")
            kratos_utils.DeleteFileIfExisting("./steady_cylinder_test.post.bin")
            kratos_utils.DeleteFileIfExisting("./tests.post.lst")

    def tearDown(self):
        pass

def _getTimeAveragedDrag(direction,drag_file_name):
    time_steps, reactions = _readDrag(drag_file_name)
    total_drag = 0.0
    for reaction in reactions:
        total_drag += reaction[0]*direction[0]+reaction[1]*direction[1]+reaction[2]*direction[2]
    if len(time_steps) > 1:
        delta_time = time_steps[1] - time_steps[0]
        total_drag *= delta_time
    return total_drag

def _readDrag(filename):
    with open(filename, "r") as file_input:
        lines = file_input.readlines()
    time_steps = []
    reaction = []
    for line in lines:
        line = line.strip()
        if len(line) == 0 or line[0] == '#':
            continue
        time_step_data = [float(v) for v in line.split()]
        time, fx, fy, fz = time_step_data
        time_steps.append(time)
        reaction.append([fx, fy, fz])
    return time_steps, reaction

if __name__ == '__main__':
    KratosUnittest.main()
