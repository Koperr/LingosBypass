cd "$(dirname "$0")"
cd ..

rm -rf libs/json
git clone https://github.com/nlohmann/json.git libs/json