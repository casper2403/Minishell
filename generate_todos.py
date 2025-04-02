import os
import re

SOURCE_DIRS = ["src", "include"]
TODO_PATTERN = re.compile(r"(//|#)\s*TODO[:]?\s*(.*)")

def extract_todos():
    todos = []
    for root, _, files in os.walk("."):
        if not any(d in root for d in SOURCE_DIRS):
            continue
        for file in files:
            if not file.endswith((".c", ".h", ".py", ".cpp")):
                continue
            path = os.path.join(root, file)
            with open(path, "r", encoding="utf-8") as f:
                for i, line in enumerate(f, 1):
                    match = TODO_PATTERN.search(line)
                    if match:
                        todos.append(f"- `{path}:{i}` — {match.group(2).strip()}")
    return todos

def update_readme(todos):
    if not os.path.exists("README.md"):
        with open("README.md", "w", encoding="utf-8") as f:
            f.write("# TODOs\n\n<!-- TODO-START -->\n<!-- TODO-END -->\n")

    with open("README.md", "r", encoding="utf-8") as f:
        lines = f.readlines()

    try:
        start = lines.index("<!-- TODO-START -->\n") + 1
        end = lines.index("<!-- TODO-END -->\n")
    except ValueError:
        raise RuntimeError("README.md must contain <!-- TODO-START --> and <!-- TODO-END --> markers.")

    new_block = [todo + "\n" for todo in todos]
    new_lines = lines[:start] + new_block + lines[end:]

    with open("README.md", "w", encoding="utf-8") as f:
        f.writelines(new_lines)
