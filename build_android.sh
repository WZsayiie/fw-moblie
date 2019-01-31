#!/bin/bash

set -e -u

cd `dirname $0`

### configuration begin
projct_dir="app_android"
flavor_nam="master"
ultima_dir="BUILD"
ultima_fil="app-master.apk"
### configutation end

origin_dir="$projct_dir/app/build/outputs/apk/$flavor_nam/release"
origin_fil="app-$flavor_nam-release.apk"

# build library
sh library/GENERATE_HEADERS.sh

pushd $projct_dir
./gradlew clean
./gradlew assembleRelease
popd

# copy apk
mv $origin_dir/$origin_fil $origin_dir/$ultima_fil
mkdir -p                   $ultima_dir
mv $origin_dir/$ultima_fil $ultima_dir