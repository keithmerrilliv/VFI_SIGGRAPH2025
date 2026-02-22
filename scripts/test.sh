source ./scripts/common.sh


SCHEME=VFISample

pushd VFISample

# only Debug builds are built for test
config=Debug

border
sectionStart xcode-${config} "Building $config ${SCHEME} with Xcode"
banner "starting XCode $config ${SCHEME} test"

# Start function to watch log for when to take a screenshot asynchronously
# capture_mock_connection_screenshot &

SCREENSHOT_PID=$!

xcodebuild \
    -scheme "${SCHEME}" \
    -destination "platform=visionOS Simulator,name=Apple Vision Pro" \
    -configuration $config \
    test \
    2>&1 | pretty
checkBuild
banner "finished Xcode $config ${SCHEME} test"
sectionEnd xcode-${config}

popd


# Send hangup signal to background tasks at end of script
if ps -p $SCREENSHOT_PID > /dev/null
then
    pkill -SIGHUP -P $SCREENSHOT_PID
fi

wait

echo