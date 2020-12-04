#include <stdio.h>
#include<stdlib.h>

int checkFile();
void readFile(int** arr);
void printArray(int** arr);
void checkGraph(int** arr );
void makeMas(int** arr,int* nodeNames,int** connect);
void seeGraph(int** connect, int* nodenames);
void deleteNode(int** arr,int n);
void vertGrade(int** arr, int** verts);
void printVerts(int** vert);
void heapSort(int** vert);
void siftDown(int** vert,int root, int bottom);

static int is_directed=0;//является ли граф ориентрированным 
static int is_weight=0;//является ли граф взвешенным
static int NumV=0;//количество вершин графа
static int NumN=0;//количество узлов графа
static char c=0;
static FILE* MatrFile;

int main(void){
  int key=0;
  int flag=1;
  if(checkFile()){
    while (flag){
      int** Arr =(int**)malloc(NumV*(sizeof(int*)));// массив содержащий матрицу
      for (int i = 0; i < NumV; i++)
        Arr[i]=(int*)malloc(NumN*(sizeof(int)));
      readFile(Arr);
      checkGraph(Arr);
      int* NodeNames=(int*)malloc(NumN*(sizeof(int)));//массив узлов
      int** Connect =(int**)malloc(NumN*(sizeof(int*)));//массив соединнных вершин
      for (int i = 0; i < NumN; i++)
        Connect[i]=(int*)malloc(2*(sizeof(int)));
      printf("Press 1 to see graph\n");
      printf("Press 2 to see matrix of a graph\n");
      printf("Press 3 to delete one node\n");
      printf("Press 4 to sort vertexes\n");
      printf("Press any other symbol to quit\n");
      scanf("%d",&key);
      makeMas(Arr,NodeNames,Connect);
      switch (key){
        case 1:
          seeGraph(Connect,NodeNames);
          break;
        case 2: 
          printArray(Arr);
          break;
       
        case 3:
          printf("Push the node\n");
          int n;
          scanf("%d",&n);
          deleteNode(Arr,n);
          break;
        
        case 4:;
          int** Verts=(int**)malloc(NumV*sizeof(int));
          for (int i = 0; i < NumV; i++)
          Verts[i]=(int*)malloc(2*sizeof(int));
          vertGrade(Arr,Verts);
          heapSort(Verts);
          printVerts(Verts);
          free(Verts);
          break;

        default:
          flag=0;
          break;
        }
      free(Arr);
      free(Connect);
      free(NodeNames);   
    }
  }
return 0;
}




// открывает файл с матрцицей и считывает ее размер
int checkFile(){
   MatrFile=fopen("matr.txt", "r");
	 if(MatrFile==NULL){
        printf("No such File");
        return 0;
     }else{
        printf("File is read\n");
        while(!feof(MatrFile)){              
            if((fgetc(MatrFile))==';'){
                NumV++;
            }
        } 
        fclose(MatrFile);
        FILE *MatrFile=fopen("matr.txt", "r");
        while((c=fgetc(MatrFile))!=';'){
            if(c==',')
                NumN++;
        }
        NumN++;
        fclose(MatrFile);
        return 1;
    }
}

// считывает матрицу из файла в массив
void readFile(int** arr){
    MatrFile=fopen("matr.txt", "r");
        for(int i=0;i<NumV;i++){
            int j=0;
            while((c=fgetc(MatrFile))!=';')
              fscanf(MatrFile,"%d",&arr[i][j++]);
          }
        fclose(MatrFile);
  }

// выводит матрицу
void printArray(int** arr){
  for (int i = 0; i < NumV; i++){
		for (int j = 0; j < NumN; j++){
			printf("%d ",arr[i][j]);
    }
    printf("\n");
  }
}

// проверяет, является ли граф ориентированным или взвешенным
void checkGraph(int** arr){
  for (int i = 0; i < NumV; i++){
		    for (int j = 0; j < NumN; j++){
            if (arr[i][j]>1)
              is_weight=1;
            if(arr[i][j]<0)
              is_directed=1;
            }
          printf("\n");
	    }  
}

// заполняем массив связзаных вершин и массив узлов
void makeMas(int** arr,int* nodeNames,int** connect ){
  for (int j = 0; j < NumN; j++){
    for (int i = 0; i < NumV; i++){
      if (arr[i][j]>0){
        if (is_weight)                        
          nodeNames[j]=arr[i][j];
        else
          nodeNames[j]=j;   
        }                             
     }
  }
  for (int j = 0; j < NumN; j++){
    int k=0;
    for (int i = 0; i < NumV; i++){
      if(arr[i][j]<0)
          connect[j][1]='a'+i;
           if (arr[i][j]>0)
              connect[j][k++]='a'+i;   
             }
        }
  for (int i = 0; i < NumN; i++){
	  for (int j = 0; j < 2; j++){
      if(!connect[i][j])
         connect[i][j]=connect[i][j-1];
	  }      
  }
}

// создает и компилирует файл на языке .dot
void seeGraph(int** connect,int* nodeNames){
  FILE *GraphFile=fopen("graph.dot", "w");
  if(GraphFile==NULL){
    printf("Problem with making a file");
  }else{
    printf("File is created\n");
    if(is_directed)
        fprintf(GraphFile,"di");
      fprintf(GraphFile,"graph Test {\n");
      for (int i = 0; i < NumN; i++){
        if(!is_directed)
         fprintf(GraphFile,"%c -- %c ",connect[i][0],connect[i][1]);
           else{
            fprintf(GraphFile,"%c -> %c ",connect[i][0],connect[i][1]);
            }
         fprintf(GraphFile,"[label= %d] \n",nodeNames[i]);                
         }
       fprintf(GraphFile,"}\n");
       fclose(GraphFile);
       system("dot -Tpng graph.dot -o graph.png");
       system("xdg-open graph.png");
    } 
  }
 
 // удаляет один узел
  void deleteNode(int** arr,int n){
   MatrFile=fopen("matr.txt", "w");
	 if(MatrFile==NULL){
        printf("No such File");
     }else{
        printf("File is opened\n");
    for (int i = 0; i < NumV; i++){
        for (int j = 0; j <NumN-1; j++){
            if(n!=j)
                fprintf(MatrFile," %d,",arr[i][j]);
            
        }
        fprintf(MatrFile," %d;\n",arr[i][NumN-1]);
    }
  }
  fclose(MatrFile);
  --NumN;
  }

  // заполняет массив вершинами и их степенями
  void vertGrade(int** arr, int** verts){
    for (int i = 0; i < NumV; i++)
      for(int j=0;j<NumN; j++){
        if (arr[i][j]==1)
        {
          verts[i][1]++;
          verts[i][0]='a'+i;
        }
      }
    }
 
  // вывод массива вершин и степеней
  void printVerts(int** verts){
    for (int i = 0; i < NumV; i++){
      printf("%c--%d\n",verts[i][0],verts[i][1]);
    } 
  }

  // функция сортировки на куче
  void heapSort(int** vert) {
  // Формируем нижний ряд пирамиды
  for (int i = (NumV / 2) - 1; i >= 0; i--)
    siftDown(vert, i, NumV - 1);
  // Просеиваем через пирамиду остальные элементы
  for (int i = NumV - 1; i >= 1; i--){
    int temp = vert[0][1];
    int tempa=vert[0][0];
    vert[0][1] = vert[i][1];
    vert[0][0]= vert[i][0];
    vert[i][1] = temp;
    vert[i][0]=tempa;
    siftDown(vert, 0, i - 1);
  }
}

// функция "просеивания" через кучу - формирование кучи
void siftDown(int** vert, int root, int bottom){
  int maxChild; // индекс максимального потомка
  int done = 0; // флаг того, что куча сформирована
  // Пока не дошли до последнего ряда
  while ((root * 2 <= bottom) && (!done)) {
    if (root * 2 == bottom)    // если мы в последнем ряду,
      maxChild = root * 2;    // запоминаем левый потомок
    // иначе запоминаем больший потомок из двух
    else if (vert[root * 2][1] > vert[root * 2 + 1][1])
      maxChild = root * 2;
    else
      maxChild = root * 2 + 1;
    // если элемент вершины меньше максимального потомка
    if (vert[root][1] < vert[maxChild][1]) {
      int temp = vert[root][1]; // меняем их местами
      int tempa=vert[root][0];
      vert[root][1] = vert[maxChild][1];
      vert[root][0] = vert[maxChild][0];
      vert[maxChild][1] = temp;
      vert[maxChild][0] = tempa;
      root = maxChild;
    }
    else // иначе
      done = 1; // пирамида сформирована
  }
}