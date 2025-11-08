import os
import re
os.system('')
root_dir = r"../../../.."
output_file = "merged_no_block.txt"
extensions = [".cpp", ".hpp", ".h"]

def merge_file():
    total_files = 0
    cpp_count = 0
    hpp_count = 0
    total_lines = 0
    class_count = 0

    class_pattern = re.compile(r'^\s*(class|struct)\s+\w+', re.MULTILINE)

    with open(output_file, "w", encoding="utf-8") as out_f:
        for dirpath, _, filenames in os.walk(root_dir):
            if "SDL_Lib" in dirpath or "ThirdParty" in dirpath:
                continue

            for filename in filenames:
                ext = os.path.splitext(filename)[1].lower()
                if ext in extensions:
                    file_path = os.path.join(dirpath, filename)
                    try:
                        with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
                            lines = f.readlines()
                            non_empty_lines = [line for line in lines if line.strip()]
                            content = ''.join(non_empty_lines)
                            out_f.writelines(non_empty_lines)

                            # 统计信息
                            total_files += 1
                            total_lines += len(non_empty_lines)
                            class_count += len(class_pattern.findall(content))
                            if ext == ".cpp":
                                cpp_count += 1
                            elif ext in (".hpp", ".h"):
                                hpp_count += 1
                    except Exception as e:
                        print(f"跳过 {file_path}: {e}")

    print(f"\n✅ 合并完成！输出文件: {output_file}")
    print(f"📦 总文件数: {total_files}")
    print(f"🔹 C++ 源文件(.cpp): {cpp_count}")
    print(f"🔸 头文件(.hpp/.h): {hpp_count}")
    print(f"🧾 总行数(非空): {total_lines}")
    print(f"🏷️ 类 / 结构体定义数: {class_count}")

if __name__ == '__main__':
    ans = input("是否执行 merge_file？(y/n): ").strip().lower()
    if ans == 'y':
        merge_file()
    else:
        print("命令已取消")