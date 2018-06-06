#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<tiff-4.0.3/libtiff/tiffio.h>

const int Isize = 512;
const int Jsize = 512;


void mode();
void read_file(unsigned char *);
void tiff_save(unsigned char *);
void tiff_out(unsigned char *);
voif change_step(unsigned char *,unsigned char *);

int main()	//メイン
{
	mode();
	return 0;
}


void mode()	//モード選択
{
	int i;
	unsigned char dat_md[Isize][Jsize];
	unsigned char dat_temp[Isize][Jsize];
	
	for(;;)
	{
		printf("モードを選択してください\n");
		printf("1.画像読み込み\n諧調変化\n濃度変換\nガンマ変換\nヒストグラム\nセーブ\n終了\n");
		printf("モードNo. = ");
		scanf("%d",i);
		switch(i)
		{
			case 1:
				read_file(&dat_md);
				break;
			case 2:
				change_step(&dat_md,&dat_temp);
				break;
			case 3:
				noudo_henkan(&dat_md);
				break;
			case 4:
				ganma_henkan(&dat_md);
				break;
			case 5:
				histgram(&dat_md);
				break;
			case 6:
				tiff_save(&dat_temp);
				break;
			case 7:
				break;
			default:
				break;
		}
	}
	exit(1);
}


void read_file(unsigned char *dat_rf)	//画像読み込み
{ 
	FILE *fi;
	char *name;

	printf("File Name :");
	if(scanf("%ms",&name) == NULL)
	{
		printf("Read Error\n");
		exit(1);
	}
	
	if((fi=fopen(name,"r"))==NULL)
	{
		printf("Read Error\n");
		exit(1);
	}
	
	fread(dat,1,Isize*Jsize,fi);
	
	free(name);
	fclose(fi);
}


void tiff_save(unsigned char *dat_temp_ts)
{
	TIFF *image;
	unsigned char buffer[Isize*Jsize];
	
	int k=0;
	char *save_fname;
	
	printf("Save file name : ");
	scanf("%ms",save_fname);
	
	for(int i=0;i<Isize;i++)
	{
		for(int j=0;j<Jsize;j++)
		{
			buffer[k]=dat_temp_ts[i][j];
			k++;
		}
	}
	
	// Open the TIFF file
	if((image = TIFFOpen(save_fname, "w")) == NULL){
		printf("Could not open output file for writing\n");
		exit(42);
	}

	// We need to set some values for basic tags before we can add any data
	TIFFSetField(image, TIFFTAG_IMAGEWIDTH, Isize);	//画像の幅
	TIFFSetField(image, TIFFTAG_IMAGELENGTH, Jsize);	//画像の高さ
	TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 8);	//ピクセルの色深度（ビット数）
	TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);	//ピクセルあたりの色数
	//TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, Jsize);	//数行をひとまとめにして1ストリップと定義している場合

	TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);	//画像の種別(0:mono,1:gray,2:RGB,3:index color,6:YCrCb)
	TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
	TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);	//このタグの数値が1（CONTIG）ならばBIP配列、2（SEPARATE）ならBIL配列

	TIFFSetField(image, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);	//原点開始位置を指定
	//TIFFSetField(image, TIFFTAG_XRESOLUTION, 512.0);	//Xにおけるpixels/resolutionを意味する。実寸のサイズと画面上のサイズの比を指定dpi
	//TIFFSetField(image, TIFFTAG_YRESOLUTION, 512.0);
	//TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_CENTIMETER);
  
	// Write the information to the file
	TIFFWriteEncodedStrip(image, 0, buffer, Isize * Jsize);	//画像データをTiff形式のファイルに保存

	// Close the file
	TIFFClose(image);
	close(fname);
}


void tiff_out(unsigned char *dat_o)
{
	TIFF *image;
	unsigned char buffer[Isize*Jsize];
	
	int k=0;

	for(int i=0;i<Isize;i++)
	{
		for(int j=0;j<Jsize;j++)
		{
			buffer[k]=dat_temp_ts[i][j];
			k++;
		}
	}
	
	// Open the TIFF file
	if((image = TIFFOpen("output", "w")) == NULL){
		printf("Could not open output file for writing\n");
		exit(42);
	}

	// We need to set some values for basic tags before we can add any data
	TIFFSetField(image, TIFFTAG_IMAGEWIDTH, Isize);	//画像の幅
	TIFFSetField(image, TIFFTAG_IMAGELENGTH, Jsize);	//画像の高さ
	TIFFSetField(image, TIFFTAG_BITSPERSAMPLE, 8);	//ピクセルの色深度（ビット数）
	TIFFSetField(image, TIFFTAG_SAMPLESPERPIXEL, 1);	//ピクセルあたりの色数
	//TIFFSetField(image, TIFFTAG_ROWSPERSTRIP, Jsize);	//数行をひとまとめにして1ストリップと定義している場合

	TIFFSetField(image, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
	TIFFSetField(image, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);	//画像の種別(0:mono,1:gray,2:RGB,3:index color,6:YCrCb)
	TIFFSetField(image, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
	TIFFSetField(image, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);	//このタグの数値が1（CONTIG）ならばBIP配列、2（SEPARATE）ならBIL配列

	TIFFSetField(image, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);	//原点開始位置を指定
	//TIFFSetField(image, TIFFTAG_XRESOLUTION, 512.0);	//Xにおけるpixels/resolutionを意味する。実寸のサイズと画面上のサイズの比を指定dpi
	//TIFFSetField(image, TIFFTAG_YRESOLUTION, 512.0);
	//TIFFSetField(image, TIFFTAG_RESOLUTIONUNIT, RESUNIT_CENTIMETER);
  
	// Write the information to the file
	TIFFWriteEncodedStrip(image, 0, buffer, Isize * Jsize);	//画像データをTiff形式のファイルに保存

	// Close the file
	TIFFClose(image);
	close(fname);
	
	printf("Please Cheak Output File And Pless Enterkey");
	//system("eog output.tiff");
	//無限ループに陥るかもなので修正予定?
	for (;;)
	{
		int key = WaitKey();
		if (key == KEY_INPUT_RETURN)
		{
			break;
		}
	}
	//system("rm -f output.tiff");
}


void change_step(unsigned char *dat_cs,unsigned char *dat_cs_temp)
{
    int s,v;
    unsigned char c;
    printf("Number of steps : ");	//階調数の入力
    scanf("%d",&s);
    v=256/s;

    for(int i=0;i<Isize;i++){
        for(int j=0;j<Jsize;j++){
            c=(unsigned char)((dat_cs[i][j]/256.)*s);
            dat_cs_temp[i][j]=c*v;
        }
    }
	
	tiff_out(&dat_cs_temp);	//ややこしい!
}