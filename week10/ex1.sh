#!/bin/bash

# Check if a directory path is provided
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path_to_watch>"
    exit 1
fi

# Assign the provided directory path to a variable
WATCHED_PATH=$1

# Start the monitor program in the background
./monitor "$WATCHED_PATH" &
MONITOR_PID=$!

# Give the monitor program a second to start up
sleep 1

# Run ex1.c program to create files and links
./ex1 "$WATCHED_PATH"

# Give some time for the events to be processed
sleep 1

# Execute the test commands from the gist
mkdir "$WATCHED_PATH/folder1"
echo "This is a test file." > "$WATCHED_PATH/f1.txt"
echo "Another test file." > "$WATCHED_PATH/f2.txt"
ls "$WATCHED_PATH/folder1"
cat "$WATCHED_PATH/f1.txt"
echo "Hello" >> "$WATCHED_PATH/f2.txt"
touch "$WATCHED_PATH/f1.txt"
rm "$WATCHED_PATH/f1.txt"
chmod u=w "$WATCHED_PATH/folder1"
rm -r "$WATCHED_PATH/folder1"

# Give some time for the events to be processed
sleep 1

# Stop the monitor program by sending SIGTERM
kill -SIGTERM "$MONITOR_PID"

# Wait for the monitor program to exit
wait "$MONITOR_PID"

# The script has finished
echo "ex1.sh test script has completed."
