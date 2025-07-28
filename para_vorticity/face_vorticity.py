# state file generated using paraview version 5.10.0

# uncomment the following three lines to ensure this script works in future versions
#import paraview
#paraview.compatibility.major = 5
#paraview.compatibility.minor = 10

#### import the simple module from the paraview
from paraview.simple import *

def Bloop(path, dump):
	#### disable automatic camera reset on 'Show'
	paraview.simple._DisableFirstRenderCameraReset()

	# ----------------------------------------------------------------
	# setup views used in the visualization
	# ----------------------------------------------------------------

	# get the material library
	materialLibrary1 = GetMaterialLibrary()

	# Create a new 'Render View'
	renderView1 = CreateView('RenderView')
	renderView1.ViewSize = [1559, 769]
	renderView1.InteractionMode = '2D'
	renderView1.AxesGrid = 'GridAxes3DActor'
	renderView1.OrientationAxesVisibility = 0
	renderView1.CenterOfRotation = [4.0, -1.8849070370197296, 0.0]
	renderView1.StereoType = 'Crystal Eyes'
	renderView1.CameraPosition = [0.20190186259992587, 0.05072956797833007, 17.482400692593064]
	renderView1.CameraFocalPoint = [0.20190186259992587, 0.05072956797833007, 0.0]
	renderView1.CameraFocalDisk = 1.0
	renderView1.CameraParallelScale = 0.10149061732788595
	renderView1.UseColorPaletteForBackground = 0
	renderView1.Background = [1.0, 1.0, 1.0]
	renderView1.BackEnd = 'OSPRay raycaster'
	renderView1.OSPRayMaterialLibrary = materialLibrary1

	SetActiveView(None)

	# ----------------------------------------------------------------
	# setup view layouts
	# ----------------------------------------------------------------

	# create new layout object 'Layout #1'
	layout1 = CreateLayout(name='Layout #1')
	layout1.AssignView(0, renderView1)
	layout1.SetSize(1559, 769)

	# ----------------------------------------------------------------
	# restore active view
	SetActiveView(renderView1)
	# ----------------------------------------------------------------

	# ----------------------------------------------------------------
	# setup the data processing pipelines
	# ----------------------------------------------------------------

	# create a new 'HyperTreeGrid Reader'
	htghtg = HyperTreeGridReader(registrationName='htg.htg', FileNames=[path+dump])

	# create a new 'HyperTreeGrid To Dual Grid'
	hyperTreeGridToDualGrid1 = HyperTreeGridToDualGrid(registrationName='HyperTreeGridToDualGrid1', Input=htghtg)

	# create a new 'Contour'
	tpool_boundary = Contour(registrationName='Tpool_boundary', Input=hyperTreeGridToDualGrid1)
	tpool_boundary.ContourBy = ['POINTS', 'Tpool']
	tpool_boundary.Isosurfaces = [0.5]
	tpool_boundary.PointMergeMethod = 'Uniform Binning'

	# create a new 'Contour'
	tdrop_boundary = Contour(registrationName='Tdrop_boundary', Input=hyperTreeGridToDualGrid1)
	tdrop_boundary.ContourBy = ['POINTS', 'Tdrop']
	tdrop_boundary.Isosurfaces = [0.5]
	tdrop_boundary.PointMergeMethod = 'Uniform Binning'

	# create a new 'Threshold'
	noair_Threshold1 = Threshold(registrationName='Noair_Threshold1', Input=hyperTreeGridToDualGrid1)
	noair_Threshold1.Scalars = ['POINTS', 'f']
	noair_Threshold1.LowerThreshold = 1e-08
	noair_Threshold1.UpperThreshold = 1.0

	# create a new 'Contour'
	interface = Contour(registrationName='interface', Input=hyperTreeGridToDualGrid1)
	interface.ContourBy = ['POINTS', 'f']
	interface.Isosurfaces = [0.5]
	interface.PointMergeMethod = 'Uniform Binning'

	# ----------------------------------------------------------------
	# setup the visualization in view 'renderView1'
	# ----------------------------------------------------------------

	# show data from interface
	interfaceDisplay = Show(interface, renderView1, 'GeometryRepresentation')

	# trace defaults for the display properties.
	interfaceDisplay.Representation = 'Surface'
	interfaceDisplay.AmbientColor = [0.0, 0.0, 0.0]
	interfaceDisplay.ColorArrayName = ['POINTS', '']
	interfaceDisplay.DiffuseColor = [0.0, 0.0, 0.0]
	interfaceDisplay.LineWidth = 3.0
	interfaceDisplay.SelectTCoordArray = 'None'
	interfaceDisplay.SelectNormalArray = 'None'
	interfaceDisplay.SelectTangentArray = 'None'
	interfaceDisplay.OSPRayScaleArray = 'f'
	interfaceDisplay.OSPRayScaleFunction = 'PiecewiseFunction'
	interfaceDisplay.SelectOrientationVectors = 'None'
	interfaceDisplay.ScaleFactor = 0.8
	interfaceDisplay.SelectScaleArray = 'f'
	interfaceDisplay.GlyphType = 'Arrow'
	interfaceDisplay.GlyphTableIndexArray = 'f'
	interfaceDisplay.GaussianRadius = 0.04
	interfaceDisplay.SetScaleArray = ['POINTS', 'f']
	interfaceDisplay.ScaleTransferFunction = 'PiecewiseFunction'
	interfaceDisplay.OpacityArray = ['POINTS', 'f']
	interfaceDisplay.OpacityTransferFunction = 'PiecewiseFunction'
	interfaceDisplay.DataAxesGrid = 'GridAxesRepresentation'
	interfaceDisplay.PolarAxes = 'PolarAxesRepresentation'
	#interfaceDisplay.SelectInputVectors = [None, '']
	#interfaceDisplay.WriteLog = ''

	# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
	interfaceDisplay.ScaleTransferFunction.Points = [0.5, 0.0, 0.5, 0.0, 0.5001220703125, 1.0, 0.5, 0.0]

	# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
	interfaceDisplay.OpacityTransferFunction.Points = [0.5, 0.0, 0.5, 0.0, 0.5001220703125, 1.0, 0.5, 0.0]

	# show data from noair_Threshold1
	noair_Threshold1Display = Show(noair_Threshold1, renderView1, 'UnstructuredGridRepresentation')

	# get color transfer function/color map for 'omega'
	omegaLUT = GetColorTransferFunction('omega')
	omegaLUT.AutomaticRescaleRangeMode = 'Never'
	omegaLUT.RGBPoints = [-500.0, 0.231373, 0.298039, 0.752941, 0.0, 0.865003, 0.865003, 0.865003, 500.0, 0.705882, 0.0156863, 0.14902]
	omegaLUT.ScalarRangeInitialized = 1.0

	# get opacity transfer function/opacity map for 'omega'
	omegaPWF = GetOpacityTransferFunction('omega')
	omegaPWF.Points = [-500.0, 0.0, 0.5, 0.0, 500.0, 1.0, 0.5, 0.0]
	omegaPWF.ScalarRangeInitialized = 1

	# trace defaults for the display properties.
	noair_Threshold1Display.Representation = 'Surface'
	noair_Threshold1Display.ColorArrayName = ['POINTS', 'omega']
	noair_Threshold1Display.LookupTable = omegaLUT
	noair_Threshold1Display.SelectTCoordArray = 'None'
	noair_Threshold1Display.SelectNormalArray = 'None'
	noair_Threshold1Display.SelectTangentArray = 'None'
	noair_Threshold1Display.OSPRayScaleArray = 'u'
	noair_Threshold1Display.OSPRayScaleFunction = 'PiecewiseFunction'
	noair_Threshold1Display.SelectOrientationVectors = 'None'
	noair_Threshold1Display.ScaleFactor = 0.8
	noair_Threshold1Display.SelectScaleArray = 'u'
	noair_Threshold1Display.GlyphType = 'Arrow'
	noair_Threshold1Display.GlyphTableIndexArray = 'u'
	noair_Threshold1Display.GaussianRadius = 0.04
	noair_Threshold1Display.SetScaleArray = ['POINTS', 'u']
	noair_Threshold1Display.ScaleTransferFunction = 'PiecewiseFunction'
	noair_Threshold1Display.OpacityArray = ['POINTS', 'u']
	noair_Threshold1Display.OpacityTransferFunction = 'PiecewiseFunction'
	noair_Threshold1Display.DataAxesGrid = 'GridAxesRepresentation'
	noair_Threshold1Display.PolarAxes = 'PolarAxesRepresentation'
	noair_Threshold1Display.ScalarOpacityFunction = omegaPWF
	noair_Threshold1Display.ScalarOpacityUnitDistance = 0.2825506026327536
	noair_Threshold1Display.OpacityArrayName = ['POINTS', 'u']
	#noair_Threshold1Display.SelectInputVectors = [None, '']
	#noair_Threshold1Display.WriteLog = ''

	# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
	noair_Threshold1Display.ScaleTransferFunction.Points = [-0.6811851263046265, 0.0, 0.5, 0.0, 10.265732765197754, 1.0, 0.5, 0.0]

	# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
	noair_Threshold1Display.OpacityTransferFunction.Points = [-0.6811851263046265, 0.0, 0.5, 0.0, 10.265732765197754, 1.0, 0.5, 0.0]

	# ----------------------------------------------------------------
	# setup color maps and opacity mapes used in the visualization
	# note: the Get..() functions create a new object, if needed
	# ----------------------------------------------------------------

	# ----------------------------------------------------------------
	# restore active source
	SetActiveSource(None)
	# ----------------------------------------------------------------

	# save screenshot
	SaveScreenshot(dump + '.png', renderView1, ImageResolution=[6236, 3076])

	if __name__ == '__main__':
	    # generate extracts
	    SaveExtracts(ExtractsOutputDirectory='extracts')
