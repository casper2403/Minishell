
import os
import re

SOURCE_DIRS = ["src", "include"]
COMMENT_PATTERN = re.compile(r"\s*(//|#)\s?(.*)")
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
                lines = f.readlines()

            i = 0
            while i < len(lines):
                line = lines[i]
                todo_match = TODO_PATTERN.search(line)
                if todo_match:
                    # Start collecting adjacent comments (above)
                    comments = []
                    j = i - 1
                    while j >= 0 and COMMENT_PATTERN.match(lines[j]):
                        content = COMMENT_PATTERN.match(lines[j]).group(2).strip()
                        if "TODO" not in content:
                            comments.insert(0, content)
                        j -= 1

                    # Add inline TODO comment if it has non-TODO content
                    content = todo_match.group(2).strip()
                    if content and "TODO" not in content:
                        comments.append(content)

                    # Collect adjacent comments (below)
                    j = i + 1
                    while j < len(lines) and COMMENT_PATTERN.match(lines[j]):
                        content = COMMENT_PATTERN.match(lines[j]).group(2).strip()
                        if "TODO" not in content:
                            comments.append(content)
                        j += 1

                    if comments:  # skip empty blocks
                        block = f"- `{path}:{i+1}`\n```c\n" + "\n".join(comments) + "\n```\n"
                        todos.append(block)
                    i = j  # Skip processed comment block
                else:
                    i += 1
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

    new_block = [todo + "\n\n" for todo in todos]
    new_lines = lines[:start] + new_block + lines[end:]

    with open("README.md", "w", encoding="utf-8") as f:
        f.writelines(new_lines)

if __name__ == "__main__":
    todos = extract_todos()
    update_readme(todos)
