#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(const char *path, const char *filename)
{
    int fd;
    char buf[512];  // 存储路径名
    char *p;        // 活动指针，指向路径尾
    struct dirent de; 
    struct stat st; // 存储文件信息

    // 打开目录，并赋值fd
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    // 将fd关联的文件信息存储至st中
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot fstat %s\n", path);
        close(fd);
        exit(1);
    }

    // 第一个参数必须是目录
    if (st.type != T_DIR)
    {
        fprintf(2, "usage: find <dirname> <filename>\n");
        exit(1);
    }

    // 判断路径名是否过长，包含路径中的"/"
    if ((strlen(path) + 1 + DIRSIZ + 1) > sizeof(buf))
    {
        fprintf(2, "find: path is too long\n");
        close(fd);
        exit(1);
    }

    strcpy(buf, path);
    // P指向路径名尾
    p = buf + strlen(buf);
    *p = '/';
    p++;

    // 以下不太理解
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0) continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            fprintf(2, "find: cannot stat %s\n", buf);
            continue;
        }

        // 不在"."和".."目录中递归
        if (st.type == T_DIR && strcmp(p, ".") != 0 && strcmp(p, "..") != 0)
        {
            find(buf, filename);
        }
        else if (strcmp(filename, p) == 0)
        {
            printf("%s\n", buf);
        }
    }
    
    close(fd);

}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        // 判断输入的参数个数
        fprintf(2, "Usge: find <dirname> <filename>\n");
        exit(1);
    }

    // 查找
    find(argv[1], argv[2]);

    exit(0);
}