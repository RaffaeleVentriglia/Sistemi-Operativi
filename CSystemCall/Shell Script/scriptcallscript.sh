#!/bin/bash
echo "Script $0 going to call another script"
bash ./scripttocall.sh # with / we specify the path (in this case, the same)
echo "Script $0 exiting."
exit 0
