The OrbConnect client is a Java(TM) program used for demonstrating communication with Orbiter
via the OrbConnect plugin.

To execute the client, perform the following steps.
1. Download and Install the Java runtime (JRE). It can be downloaded from www.java.sun.com, specifically
https://cds.sun.com/is-bin/INTERSHOP.enfinity/WFS/CDS-CDS_Developer-Site/en_US/-/USD/ViewProductDetail-Start?ProductRef=jdk-6u6-oth-JPR@CDS-CDS_Developer
2. Create an environment variable called JAVA_HOME. Point it at the directory where
you installed JAVA.
3. Add %JAVA_HOME%\bin (windows) or $JAVA_HOME/bin (*nix) to your path.
4. Go to the Orbiter plugins/OrbConnect folder and double click on OrbConnectClient.jar

The program consists of a text field in which to enter commands to send to Orbiter,
an area where the responses will be displayed, and 5 buttons.

Send - sends the entered command to Orbiter.
Get Name - Sets the command to get the focus vessels name in the text field.
HUD Color - sets the command to toggle the HUD color in the text field.
Eng Status - sets the command to get the focus vessels engine status in the text field.

The command buttons won't actually send the command, to give you the opportunity to
modify the command if you want. Press the Send button when ready.

When the program starts, it will attempt to connect to Orbiter. The send button
will remain disabled and gray until a connection is made, at which point the
button will turn green. The program won't be able to communicate while Orbiter
is in the Launchpad, and if you quit into the Launchpad the Send button will remain
green until an attempt is made to send a command.

