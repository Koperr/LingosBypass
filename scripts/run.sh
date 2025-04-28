cd "$(dirname "$0")"
cd ..

OS=$(uname)

echo "Running on: "$OS" OS"

if [ "$OS" = "Darwin" ]; then
    # macOS
    EXEC_PATH="bin/Darwin/"
elif [ "$OS" = "Linux" ]; then
    # Linux
    EXEC_PATH="bin/Linux/"
else
    echo "Unsupported OS: $OS"
    exit 1
fi

cd $EXEC_PATH
sudo ./LingosBypass
cd ../..