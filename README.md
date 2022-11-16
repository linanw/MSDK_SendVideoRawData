# MSDK_SendVideoRawData

A Windows C++ Application demostrate Zoom Meeting SDK sending video raw data as user's video source to a Zoom Meeting.

## Install vcpkg for adding dependency libs.

```
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
```

## Add dependency libs

```
./vcpkg install jsoncpp
./vcpkg install opencv

```

## Clone the poject source code

```
git clone https://github.com/linanw/MSDK_SendVideoRawData.git
```

## Add config to config.json

```
{
  "sdk_jwt": "<your_sdk_jwt>",
  "meeting_number": <meeting_number_to_join>,
  "passcode": "<passcode>",
  "video_source": "Big_Buck_Bunny_1080_10s_1MB.mp4"
}
```

The app will try to join the meeting follow the Meeting Number you specified in the config.json. Specify a video source for sending to the meeting as a vitual video source. The video source could be a file path or a stream url. A video file name, "Big_Buck_Bunny_1080_10s_1MB.mp4" is set by default, which is video file's name included in project source code.

## Open and Run Project

Open "MSDK_SendVideoRawData.vcxproj" file from Visual Studio 2022.

Hit F5 or click from menu "Debug" -> "Start Debugging" to launch the application.
