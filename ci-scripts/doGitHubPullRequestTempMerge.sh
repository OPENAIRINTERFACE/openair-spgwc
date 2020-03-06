#!/bin/bash
#/*
# * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
# * contributor license agreements.  See the NOTICE file distributed with
# * this work for additional information regarding copyright ownership.
# * The OpenAirInterface Software Alliance licenses this file to You under
# * the OAI Public License, Version 1.1  (the "License"); you may not use this file
# * except in compliance with the License.
# * You may obtain a copy of the License at
# *
# *      http://www.openairinterface.org/?page_id=698
# *
# * Unless required by applicable law or agreed to in writing, software
# * distributed under the License is distributed on an "AS IS" BASIS,
# * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# * See the License for the specific language governing permissions and
# * limitations under the License.
# *-------------------------------------------------------------------------------
# * For more information about the OpenAirInterface (OAI) Software Alliance:
# *      contact@openairinterface.org
# */

function usage {
    echo "OAI GitHub Pull Request script that creates a temporary merge commit locally"
    echo "   Original Author: Raphael Defosseux"
    echo ""
    echo "Usage:"
    echo "------"
    echo ""
    echo "    doGitHubPullRequestTempMerge.sh [OPTIONS] [MANDATORY_OPTIONS]"
    echo ""
    echo "Mandatory Options:"
    echo "------------------"
    echo ""
    echo "    --src-branch #### OR -sb ####"
    echo "    Specify the source branch of the merge request."
    echo ""
    echo "    --src-commit #### OR -sc ####"
    echo "    Specify the source commit ID (SHA-1) of the merge request."
    echo ""
    echo "    --target-branch #### OR -tb ####"
    echo "    Specify the target branch of the merge request (usually develop)."
    echo ""
    echo "    --target-commit #### OR -tc ####"
    echo "    Specify the target commit ID (SHA-1) of the merge request."
    echo ""
    echo "Options:"
    echo "--------"
    echo "    --help OR -h"
    echo "    Print this help message."
    echo ""
}

if [ $# -ne 8 ] && [ $# -ne 1 ]
then
    echo "Syntax Error: not the correct number of arguments"
    echo ""
    usage
    exit 1
fi

checker=0
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -h|--help)
    shift
    usage
    exit 0
    ;;
    -sb|--src-branch)
    SOURCE_BRANCH="$2"
    let "checker|=0x1"
    shift
    shift
    ;;
    -sc|--src-commit)
    SOURCE_COMMIT_ID="$2"
    let "checker|=0x2"
    shift
    shift
    ;;
    -tb|--target-branch)
    TARGET_BRANCH="$2"
    let "checker|=0x4"
    shift
    shift
    ;;
    -tc|--target-commit)
    TARGET_COMMIT_ID="$2"
    let "checker|=0x8"
    shift
    shift
    ;;
    *)
    echo "Syntax Error: unknown option: $key"
    echo ""
    usage
    exit 1
esac

done

echo "Source Branch is    : $SOURCE_BRANCH"
echo "Source Commit ID is : $SOURCE_COMMIT_ID"
echo "Target Branch is    : $TARGET_BRANCH"
echo "Target Commit ID is : $TARGET_COMMIT_ID"

if [ $checker -ne 15 ]
then
    echo ""
    echo "Syntax Error: missing option"
    echo ""
    usage
    exit 1
fi

git config user.email "jenkins@openairinterface.org"
git config user.name "OAI Jenkins"

git checkout -f $SOURCE_COMMIT_ID
# Keeping the source commit for the GitHub notifications
git rev-parse HEAD > .git/current-commit
# Keeping the committer email for the GitHub notifications
git log -1 --pretty=format:"%ce" > .git/commit-email
# Workaround for GitHub merge button issue
if [ `egrep -c "noreply@github.com" .git/commit-email` -eq 1 ]
then
    echo "raphael.defosseux@openairinterface.org" > .git/commit-email
fi

# Doing a temporary merge
git merge --ff $TARGET_COMMIT_ID -m "Temporary merge for CI - from $SOURCE_BRANCH to $TARGET_BRANCH"

STATUS=`git status | egrep -c "You have unmerged paths.|fix conflicts"`
if [ $STATUS -ne 0 ]
then
    echo "There are merge conflicts.. Cannot perform further build tasks"
    STATUS=-1
fi
exit $STATUS
