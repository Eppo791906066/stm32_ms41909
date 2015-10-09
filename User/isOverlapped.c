 
typedef struct rectangle  
{  
    unsigned long centerX;  //���ĵ�����
    unsigned long centerY;  
    unsigned long width;  	//����
    unsigned long height;  
}Rectangle;  

#defin MIN(a,b)		(unsigned long)((a<b)?a:b)

unsigned long fabs(unsigned long a,unsigned long b)
{
	unsigned long val;
	
	val = (a>b)?(a-b):(b-a);
	
	return val;
}

/* ˼·
 * Ҫ���������ص���������Ƿֱ���x��y�����������ε����ĵ��ľ��붼С�ڵ���(x1+x2)/2 �� (y1+y2)/2
 * ����������һ�������ص�
 */
bool areTwoRectsOverlapped(Rectangle rect1, Rectangle rect2)  
{  
    unsigned long verticalDistance;    //��ֱ����
    unsigned long horizontalDistance;  //ˮƽ����
	
    verticalDistance = fabs(rect1.centerX,rect2.centerX);  
    horizontalDistance = fabs(rect1.centerY,rect2.centerY);  
  
    unsigned long verticalThreshold;   //�����η���Ĵ�ֱ�ٽ�ֵ
    unsigned long horizontalThreshold; //�����η����ˮƽ�ٽ�ֵ
	
    verticalThreshold = (rect1.height + rect2.height)/2;  
    horizontalThreshold = (rect1.width + rect2.height)/2;  
  
    if(verticalDistance > verticalThreshold || horizontalDistance > horizontalThreshold)  
        return false;  
  
    return true;  
}  

unsigned long rect1_ori_x;
unsigned long rect1_ori_y;
unsigned long rect1_end_x;
unsigned long rect1_end_y;
unsigned long rect2_ori_x;
unsigned long rect2_ori_x;
unsigned long rect2_end_x;
unsigned long rect2_end_x;
int main()  
{  
    Rectangle rect1;  
    Rectangle rect2;  
  
		rect1.centerX = fabs(rect1_ori_x,rect1_end_x)/2 + MIN(rect1_ori_x,rect1_end_x);
		rect1.centerY = fabs(rect1_ori_y,rect1_end_y)/2 + MIN(rect1_ori_y,rect1_end_y);
		rect2.centerX = fabs(rect2_ori_x,rect2_end_x)/2 + MIN(rect2_ori_x,rect2_end_x);
		rect2.centerY = fabs(rect2_ori_y,rect2_end_y)/2 + MIN(rect2_ori_y,rect2_end_y);
	
		rect1.width 	= fabs(rect1_ori_x,rect1_end_x);
		rect1.height 	= fabs(rect1_ori_y,rect1_end_y);
		rect2.width 	= fabs(rect2_ori_x,rect2_end_x);
		rect2.height 	= fabs(rect2_ori_y,rect2_end_y);
	
    if(areTwoRectsOverlapped(rect1, rect2))  
    {
			printf("overlapped!");
		}
    else  
		{
			printf("not overlapped!");
		}
  
    return 0;  
}  