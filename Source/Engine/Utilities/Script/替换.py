import os

# === 配置部分 ===
root_dir = "../../../.."           # 要遍历的根目录
old_str = "DelegateEventDispatcher"   # 被替换的字符串
new_str = "CallDelegate"           # 替换成的字符串
extensions = [".cpp", ".hpp", ".h"]  # 文件类型

def replace_in_file(file_path):
    """在单个文件内进行替换"""
    try:
        with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
            content = f.read()

        if old_str not in content:
            return False  # 没有匹配内容就跳过

        new_content = content.replace(old_str, new_str)

        with open(file_path, "w", encoding="utf-8") as f:
            f.write(new_content)

        print(f"✅ 已替换: {file_path}")
        return True

    except Exception as e:
        print(f"⚠️ 跳过 {file_path}: {e}")
        return False


def replace_in_folder():
    count = 0
    for dirpath, _, filenames in os.walk(root_dir):
        for filename in filenames:
            ext = os.path.splitext(filename)[1].lower()
            if ext in extensions:
                file_path = os.path.join(dirpath, filename)
                if replace_in_file(file_path):
                    count += 1
    print(f"\n🎉 完成替换，共修改 {count} 个文件！")


if __name__ == "__main__":
    replace_in_folder()
    # ans = input("是否执行批量替换？(y/n): ").strip().lower()
    # if ans == 'y':
    # else:
    #     print("命令已取消～")