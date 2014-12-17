# RemoteMouse
Remotely control your mouse from an android phone.

Start the server application on the desired computer you wish to remote control.  Create a new client name and key.  Then start the client application on an android phone.  From there, enter the server's IP address and your client name and key and connect.  You should then be able to remotely control the mouse on the server from the phone.

If you're attempting to connect from outside a LAN, you'll need to forward port 48048 to the server computer on your router.

### Setup
The RemoteMouse server was built with Qt5.  It may work on versions <5.x.x but has not been tested.  The server is currently only compatible with Windows machines as the actual mouse movement code is platform specific, other platforms may be added in the future.

The RemoteMouse client should work on most any Android phone but has only been physically tested on a 4.2.2 Jellybean device.  The client project has been converted to Android Studio, but it should be able to be compiled from an Eclipse environment; not currently tested.

### TODO
* Implement mouse clicks
* Implement keyboard input
* Relay on-screen screenshot to phone.
