import subprocess
import glob
import os

avl = "../../build/avltree/./avltree"

input_dir = "input_files/"
output_dir = "output_files/"

test_files = sorted(glob.glob(os.path.join(input_dir, "test_*.in")))

for file in test_files:
    base = os.path.basename(file)
    out_file = os.path.splitext(base)[0] + ".out"
    out_path = os.path.join(output_dir, out_file)

    with open(file, "r") as fin, open(out_path, "w") as fout:
        run = subprocess.run(
            [avl],
            stdin=fin,
            text=True,
            capture_output=True
        )
        if run.returncode != 0:
            fout.write(f"ERROR: {run.stderr}\n")
        else:
            fout.write(run.stdout)
