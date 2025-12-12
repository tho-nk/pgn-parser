current_dir=$(pwd)
cd $(pwd)/pytest
python generate_test_data.py

python -m pytest ./Test*.py