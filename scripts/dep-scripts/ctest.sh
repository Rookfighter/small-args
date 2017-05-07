# ctest.sh
#
# Created on: 07 May 2017
#     Author: Fabian Meyer
#    License: MIT

export NAME="ctest"
export VERSION="master"
export URL="https://github.com/bvdberg/ctest/archive/$VERSION.zip"
export ARCHIVE="ctest.zip"
export ARCHIVE_TYPE=".zip"
export EXTRACT_NAME="ctest-$VERSION"
export EXEC="
    mv \"$NAME/ctest.h\" \"/tmp/\" ;
    rm -rf \"$NAME\" ;
    mkdir \"$NAME\" ;
    mv \"/tmp/ctest.h\" \"$NAME/\""
