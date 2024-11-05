current_dir=$(pwd)
cd $(pwd)/pytest
python3 generate_test_data.py

python3 -m pytest ./Test*.py