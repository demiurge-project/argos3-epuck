#!/bin/bash -e

#
# TODO: support input robot list
#

#
# Configuration
#

# Directory location of the argos3 core libraries
ARGOS_CORE_PATH=/usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr/lib/argos3

# Directory location of the argos3 plugin library
ARGOS_PLUGIN_PATH=../build_realepuck/plugins/robots/e-puck

# Base IP address for the e-pucks [NOTE: do not supply the . at the end!]
ROBOT_BASE_IP=10.0.1

# The user name to login into the robot
ROBOT_SSH_USER=root

# Base directory for ARGoS stuff on the robot
ROBOT_ARGOS_HOME=/home/root/ARGoS

#
# Syntax printing
#
function print_syntax() {
    echo
    echo "To upload your ssh public key on the robot for passwordless login:"
    echo "$0 key <N>"
    echo
    echo "To update the ARGoS3 core libs on robot N:"
    echo "$0 core <N>"
    echo
    echo "To update the ARGoS3 plugin libs on robot N:"
    echo "$0 plugin <N>"
    echo
    echo "To update both the ARGoS3 core and the plugin libs on robot N:"
    echo "$0 libs <N>"
    echo
    echo "To upload a file on robot N into directory <dest>:"
    echo "$0 file <path> <dest> <N>"
    echo
    echo "To take as input a list of files <list> and upload all of them on robot N into directory <dest>:"
    echo "$0 files <list> <N>"
    echo
    exit 1
}

#
# Uploads one file
#
# $1: the id of the robot
# $2: the path of the file to upload
# $3: the destination path on the robot
function upload_file() {
    ROBOT=$1
    SOURCE=$2
    DEST=$3
    scp $SOURCE $ROBOT_SSH_USER@$ROBOT_BASE_IP.$ROBOT:$DEST
}

#
# Updates the core libs
#
# $1: the id of the e-puck
function update_core() {
    ROBOT=$1
    upload_file $ROBOT $ARGOS_CORE_PATH/libargos3core_epuck.so                $ROBOT_ARGOS_HOME/lib/
    upload_file $ROBOT $ARGOS_CORE_PATH/libargos3plugin_epuck_genericrobot.so $ROBOT_ARGOS_HOME/lib/
}

#
# Updates the plugin libs
#
# $1: the id of the e-puck
function update_plugin() {
    ROBOT=$1
    upload_file $ROBOT $ARGOS_PLUGIN_PATH/libargos3plugin_epuck_epuck.so $ROBOT_ARGOS_HOME/lib/
}

#
# Uploads the ssh key onto the robot
#
# $1: the id of the e-puck
function upload_key() {
    ROBOT=$1
    ssh $ROBOT_SSH_USER@$ROBOT_BASE_IP.$ROBOT 'mkdir -p .ssh'
    if [ -e ~/.ssh/id_rsa.pub ]; then
        cat ~/.ssh/id_rsa.pub | ssh $ROBOT_SSH_USER@$ROBOT_BASE_IP.$ROBOT 'cat >> .ssh/authorized_keys'
    elif [ -e ~/.ssh/id_dsa.pub ]; then
        cat ~/.ssh/id_dsa.pub | ssh $ROBOT_SSH_USER@$ROBOT_BASE_IP.$ROBOT 'cat >> .ssh/authorized_keys'
    else
        echo "ERROR: you don't have an ssh public key. Create it with the command ssh-keygen."
        echo
        exit 1
    fi
    echo "Key added. You now can login without typing your password."
    echo
}

#
# The script starts here
#

# Check argument number
if [ $# -lt 2 ]; then
    print_syntax
fi

# Parse operation and robot id
OPERATION=$1
shift
ROBOT=$1

# Execute logic
case $OPERATION in
    core)
        update_core $ROBOT
        ;;
    plugin)
        update_plugin $ROBOT
        ;;
    libs)
        update_core $ROBOT
        update_plugin $ROBOT
        ;;
    file)
        if [ $# -ne 3 ]; then
            echo "ERROR: Wrong number of arguments passed to operation 'file'"
            echo
            print_syntax
        fi
        SOURCE=$1
        DEST=$2
        ROBOT=$3
        upload_file $ROBOT $SOURCE $DEST
        ;;
    files)
        if [ $# -ne 2 ]; then
            echo "ERROR: Wrong number o farguments passed to operation 'files'"
            echo
            print_syntax
        fi
        FILES=$1
        ROBOT=$2
        echo "reading file in '$1' for upload on robot '$ROBOT'"
        # read lines in FILE
        while read line; do
            # transform the line in an array to access words
            line=($line)
            # if the line contains smthg (i.e. size != 0)
            if [ ${#line[@]} -ne 0 ]; then
                # if the line doesn't contain 2 words, error
                if [ ${#line[@]} -ne 2 ]; then
                    echo "ERROR: Wrong syntax in file '$FILES' at line '$line', format: 'SOURCE - DEST'"
                    exit 1
                fi
                # the first word on the line is the source, the second is the destination
                SOURCE=${line[0]}
                DEST=${line[1]}
                upload_file $ROBOT $SOURCE $DEST
            fi
        done < $FILES
        ;;
    key)
        upload_key $ROBOT
        ;;
    *)
        echo "ERROR: Unknown operation \"$OPERATION\""
        echo
        print_syntax
        ;;
esac
