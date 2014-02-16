#define GESTURE_TO_USE "Click"

#include <ros/ros.h>
#include <ros/package.h>
#include <tf/transform_broadcaster.h>
#include <kdl/frames.hpp>

#include <XnOpenNI.h>
#include <XnCodecIDs.h>
#include <XnCppWrapper.h>

using std::string;

xn::Context        g_Context;
xn::DepthGenerator g_DepthGenerator;
xn::UserGenerator  g_UserGenerator;

xn::GestureGenerator g_GestureGenerator;
xn::HandsGenerator g_HandsGenerator;

void XN_CALLBACK_TYPE 
Gesture_Recognized(xn::GestureGenerator& generator,
	const XnChar* strGesture,
	const XnPoint3D* pIDPosition,
	const XnPoint3D* pEndPosition, void* pCookie)
{
	printf("Gesture recognized: %s\n", strGesture);
	g_GestureGenerator.RemoveGesture(strGesture);
	g_HandsGenerator.StartTracking(*pEndPosition);
}
void XN_CALLBACK_TYPE
Gesture_Process(xn::GestureGenerator& generator,
	const XnChar* strGesture,
	const XnPoint3D* pPosition,
	XnFloat fProgress,
	void* pCookie)
{
}
void XN_CALLBACK_TYPE
Hand_Create(xn::HandsGenerator& generator,
	XnUserID nId, const XnPoint3D* pPosition,
	XnFloat fTime, void* pCookie)
{
	printf("New Hand: %d @ (%f,%f,%f)\n", nId,
		pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE
Hand_Update(xn::HandsGenerator& generator,
	XnUserID nId, const XnPoint3D* pPosition,
	XnFloat fTime, void* pCookie)
{
	printf("Hand: %d @ (%f,%f,%f)\n", nId,
		pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE
Hand_Destroy(xn::HandsGenerator& generator,
	XnUserID nId, XnFloat fTime,
	void* pCookie)
{
	printf("Lost Hand: %d\n", nId);
	g_GestureGenerator.AddGesture(GESTURE_TO_USE, NULL);
}
#define CHECK_RC(nRetVal, what)										\
	if (nRetVal != XN_STATUS_OK)									\
	{																\
		ROS_ERROR("%s failed: %s", what, xnGetStatusString(nRetVal));\
		return nRetVal;												\
	}
int main(int argc, char **argv)
{
	ros::init(argc, argv, "openni_gesture");
    ros::NodeHandle nh;

	XnBoundingBox3D* boundingBox; 

    string configFilename = ros::package::getPath("openni_tracker") + "/openni_tracker.xml";
    XnStatus nRetVal = g_Context.InitFromXmlFile(configFilename.c_str());
    CHECK_RC(nRetVal, "InitFromXml");

	// XnStatus nRetVal = XN_STATUS_OK;
// 	Context context;
// 	nRetVal = context.Init();
// // TODO: check error code
// // Create the gesture and hands generators
	nRetVal = g_GestureGenerator.Create(g_Context);
	nRetVal = g_HandsGenerator.Create(g_Context);
// // TODO: check error code
// // Register to callbacks
	XnCallbackHandle h1, h2;
	g_GestureGenerator.RegisterGestureCallbacks(Gesture_Recognized,
		Gesture_Process,
		NULL, h1);
	g_HandsGenerator.RegisterHandCallbacks(Hand_Create, Hand_Update,
		Hand_Destroy, NULL, h2);
// // Start generating
	nRetVal = g_Context.StartGeneratingAll();
// // TODO: check error code
	nRetVal = g_GestureGenerator.AddGesture(GESTURE_TO_USE,boundingBox);

	ros::Rate r(30);
	while (TRUE)
	{
		// Update to next frame
		nRetVal = g_Context.WaitAndUpdateAll();
		// TODO: check error code
		r.sleep();
	}
// // Clean up
	g_Context.Shutdown();
}
