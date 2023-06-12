//gcc bin.c -o bin.exe

#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
	FILE *fp;
	unsigned int offset = 0; // 偏移量
	int temp_count = 0; // 记录每行已经打印的字节数量
	unsigned int bytes_count = 0; // 记录字节数量
	const unsigned char buffer[16];
	const char HexCharText[]="0123456789ABCDEF";
	int i,length,ch;

	// 检查命令行参数
	if (argc != 2) {
		fprintf(stderr,"usage: %s <filename>\n", argv[0]);
		return 1;
	}

	// 打开文件
	fp = fopen(argv[1], "rb");
	if (fp == NULL) {
		fprintf(stderr,"failed to open file %s\n", argv[1]);
		return 1;
	}

	printf("%-12s","    OFFSET"); //偏移占12列
	printf(" | ");//显示个分隔线
	for(i=0; i<16; i++) { //16个二进制码
		ch=HexCharText[i];
		printf(" 0%c",ch);
	}
	printf(" | ");//显示个分隔线
	for(i=0; i<16; i++) { //16个二进制码
		ch=HexCharText[i];
		printf(" %c",ch);
	}
	putchar('\n');

	for(i=0; i<12; i++) {
		putchar('-');
	}
	printf("-|-");//显示个分隔线
	for(i=0; i<16; i++) {
		printf("---");
	}
	printf("-|-");//显示个分隔线
	for(i=0; i<16; i++) {
		printf("--");
	}
	putchar('\n');

	offset = 0; // 偏移量
	temp_count = 0; // 记录每行已经打印的字节数量
	bytes_count = 0; // 记录字节数量
	// 读取文件并打印字节码
	while ((length=(int)fread(buffer, 1, sizeof(buffer), fp)) > 0) {
		int i;
		// 打印偏移量
		printf("(0x)");
		printf("%08X", offset);
		printf(" | ");//显示个分隔线
		for(i=0; i<length; i++) {
			// 打印字节码
			printf(" %02X", (unsigned int)buffer[i]);
		}
		//补上一行中后续的不足16个字节的部分
		for(; i<16; i++) {
			printf(" %2s"," ");
		}
		printf(" | ");//显示个分隔线
		//显示字符
		for(i=0; i<length; i++) {
			ch=(unsigned char)buffer[i];
			if(isgraph(ch)) {
				printf(" %c", ch);
			} else {
				printf(" .");
			}

			// 每16个字节一行，打印的字节数加1
			temp_count++;
			if (temp_count == 16) {
				putchar('\n'); //换行并重置计数
				temp_count = 0;
				offset += 16; //下行的偏移
			}
		}
		bytes_count+=length; //累加字节数
	}
	// 关闭文件
	fclose(fp);

	printf("\n\n%u byte(s)\n", bytes_count);

	return 0;
}

