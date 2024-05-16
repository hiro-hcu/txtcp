
/*
*第5回プログラミングゼミ
*課題2
*コマンド行で入力, 出力ファイル名を指定し, 入力ファイルに格納された文字列データを
*読み出して, 出力ファイルに出力するプログラム
*実行方法
*./txtcp [-hir] 入力ファイル名　出力ファイル名
*オプション
*-h: ヘルプ表示　-i: 出力ファイルの上書きをするかを確認する　-r: 逆順に出力
*/

#include "includes.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> //mode用


int main(int argc, char *argv[])
{
    int opt, i = 0, k = 0, flag = 0, n;
    int input_fd, output_fd; // ファイルのディスクリプタ番号
    int array_num = 2; //コマンドライン配列のインデント
    char buf[MAX];
    char cp[1];
    char messa[] = "改行\n";
    char error[] = "./実行ファイル オプション 入力ファイル名 出力ファイル名を指定してください\n";
    char help[]="基本実行方式\n./実行ファイル [-hir] 入力ファイル名 出力ファイル名\n-i: 出力ファイルの上書きの確認\n-r: 逆順に出力\n";
    char conf[]="上書きをしますか? Y/N\n";
    // オプションを利用するならオプションの後ろにコマンドラインの必要なパラメータを入れる
    // 例) ./kadiai2 -i abc

    memset(buf, 0, sizeof(buf));
    //オプション
    while ((opt = getopt(argc, argv, "hir")) != -1)
    {
        switch (opt)
        {
        case 'h':
            write(1,help, sizeof(help));
            return 0;
        case 'i':   
            while(strncmp(buf, "Y", 1)!=0 && strncmp(buf, "N", 1)!=0 ){
                memset(buf, 0, sizeof(buf));
                write(1, conf, sizeof(conf));
                read(0, buf, sizeof(buf));
                if(strncmp(buf, "Y", 1)==0){
                    write(1, "ok\n", 3);
                    break;
                }else if(strncmp(buf, "N", 1)==0){
                    write(1, "exit the program\n", 18);
                    exit(1);
                }
            }
            break;
        case 'r':   flag = 1;   break;
        default:    break;
        }
    }
    
    // 引数が足りない時または多い時
    if (argc <= 3 || argc >= 5)
    {
        write(1, error, sizeof(error));
        return 0;
    }
    // オプションがないときは、そのまま読み込んだファイルを1行ずつ書き出す
    if (flag == 0)
    {   
        //読み込みファイルオープン
        input_fd = open(argv[array_num], O_RDONLY);
        
        //読み込みファイルオープンに失敗した時
        if (input_fd == -1)
        {
            write(1, "read_file is not opened\n", 30);
            return 1;
        }
        
        //書き込みファイルをopen, 存在しなかったら作成,存在していたら内容を削除
        output_fd = open(argv[array_num + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(output_fd==-1){
            write(1, "write_file is not opened\n", 30);
            return 1;
        }
        
        memset(buf, 0, sizeof(buf));
        
        // ファイルの終端文字を指すまで回す
        while ((n = read(input_fd, cp, 1)) > 0)
        {
            buf[i]=cp[0];
            i++;
            write(1, cp, 1);
            // 改行文字を検出すると1行ファイルに書き出す
            if (cp[0] == '\n')
            {
                write(output_fd, buf, i);
                i = 0;
                memset(buf, 0, sizeof(buf));
            }
            memset(cp, 0, sizeof(cp));
        }
        close(input_fd);
        close(output_fd);
        return 0;
    }


    /*mode: openするファイルに許可を与えるモード
    O_RDONLY: 読み込み可, O_WRONLY: 書き込み可, O_RDWR: 読み書き可
    O_CREAT: ファイルが存在しない場合にファイルを作成, 
    O_TRUNC: ファイルが存在する場合にファイルの内容が消されてファイルの長さが0になる
    fdにはファイルディスクリプタ番号が返される
    */

    //逆順に上書き
    if (flag == 1)
    {
        //読み込みファイルオープン
        input_fd = open(argv[array_num], O_RDONLY);
        //読み込みファイルオープンに失敗した時
        if (input_fd == -1)
        {
            write(1, "read_file is not opened\n", 30);
            return 1;
        }
        //書き込みファイルをopen, 存在しなかったら作成,存在していたら内容を削除
        output_fd = open(argv[array_num + 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

        if(output_fd==-1){
            write(1, "write_file is not opened\n", 30);
            return 1;
        }
        
        /*
        readはあるバイトまで読み込むと次のバイトから読み出す
        (つまり、1度読み出すとファイルポインタが後ろに1つ動く)
        read(fd1, buf, sizeof(buf));
        第1引数: 操作したいファイルディスクリプタ番号
        write(fd2, buf, sizeof(buf));
        memset(buf, 0, sizeof(buf)); 初期化
        第1引数: 変数のアドレス, 第2引数: セットしたい値, 第3引数: セットしたいバイト数
        */

        memset(buf, 0, sizeof(buf));
        memset(cp, 0, sizeof(cp));
        lseek(input_fd, -2, SEEK_END); // ファイルの最後尾のポインタに

        while (1)
        {
            memset(cp, 0, sizeof(cp));
            read(input_fd, cp, 1);
            buf[i] = cp[0]; // buf配列に1行分の文字を代入

            i++;
            //ファイルポインタを移動
            k = lseek(input_fd, -2, SEEK_CUR);
            //改行文字を検出したとき
            if (cp[0] == '\n')
            {
                write(1, messa, sizeof(messa));
                rev_char(buf);
                write(output_fd, buf, i);
                memset(buf, 0, sizeof(buf));
                i = 0;
            }

            //ファイルポインタが先頭より前に来たらエラーの-1をはく
            if (k == -1)
            {
                buf[i] = '\n';
                i++;
                rev_char(buf);
                write(output_fd, buf, i);
                break;
            }
        }
        close(input_fd);
        close(output_fd);
    }
    return 0;
}
