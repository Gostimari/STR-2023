Follow this instructions to run the program with ROS1:

1 - Open the terminal and install ros noetic desktop on your Ubuntu 20.04 machine:

    sudo apt-get install ros-noetic-desktop

2 - Open terminal. Create a workspace for the package with the following code:

    $HOME (Move to home directory of your linux user)

    mkdir t2g5e6 (Create the main workspace folder, with the desired name. In this case the name chosen is "t2g5e6")

    cd t2g5e6 (Move to inside the folder)

    mkdir src (Create the "src" folder, to be used later for build)

    cd .. (Return to previous folder, that is the main folder of the workspace created before)

2 - On the GUI, copy the given package "t2g5e6_package" to the "src" folder of the point 2:

3 - Return to the terminal previously used and build the workspace with ROS Catkin tool:

    catkin_make

5 - Do a Source base of the new build:

    source devel/setup.bash

6 - Finally, to run the program start the launch file (This will open the rviz, run the package created and play the bag file given by the teacher):

    roslaunch t2g5e6_package run.launch

5 - Both PointClouds are activated by default. If you want to see just one of them, on the left menu there are "output_results" and "velodyne_points" PointClouds. Desactivate the desired one by clicking on the check box in front of each name. REMINDER: The result of the problem 6 is the "output_results".

