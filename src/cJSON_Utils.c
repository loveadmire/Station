#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "cJSON_Utils.h"

static int cJSONUtils_strcasecmp(const char *s1,const char *s2)
{
	if (!s1) return (s1==s2)?0:1;if (!s2) return 1;
	for(; tolower(*s1) == tolower(*s2); ++s1, ++s2)	if(*s1 == 0)	return 0;
	return tolower(*(const unsigned char *)s1) - tolower(*(const unsigned char *)s2);
}

/* JSON Pointer implementation: */
static int cJSONUtils_Pstrcasecmp(const char *a,const char *e)
{
	if (!a || !e) return (a==e)?0:1;
	for (;*a && *e && *e!='/';a++,e++) {
		if (*e=='~') {if (!(e[1]=='0' && *a=='~') && !(e[1]=='1' && *a=='/')) return 1;  else e++;}
		else if (tolower(*a)!=tolower(*e)) return 1;
	}
	if ((*e!=0 && *e!='/') != (*a!=0)) return 1;
	return 0;
}

static int cJSONUtils_PointerEncodedstrlen(const char *s)	{int l=0;for (;*s;s++,l++) if (*s=='~' || *s=='/') l++;return l;}

static void cJSONUtils_PointerEncodedstrcpy(char *d,const char *s)
{
	for (;*s;s++)
	{
		if (*s=='/') {*d++='~';*d++='1';}
		else if (*s=='~') {*d++='~';*d++='0';}
		else *d++=*s;
	}
	*d=0;
}

char *cJSONUtils_FindPointerFromObjectTo(cJSON *object,cJSON *target)
{
	int type=object->type,c=0;cJSON *obj=0;

	if (object==target) return strdup("");

	for (obj=object->child;obj;obj=obj->next,c++)
	{
		char *found=cJSONUtils_FindPointerFromObjectTo(obj,target);
		if (found)
		{
			if (type==cJSON_Array)
			{
				char *ret=(char*)malloc(strlen(found)+23);
				sprintf(ret,"/%d%s",c,found);
				free(found);
				return ret;
			}
			else if (type==cJSON_Object)
			{
				char *ret=(char*)malloc(strlen(found)+cJSONUtils_PointerEncodedstrlen(obj->string)+2);
				*ret='/';cJSONUtils_PointerEncodedstrcpy(ret+1,obj->string);
				strcat(ret,found);
				free(found);
				return ret;
			}
			free(found);
			return 0;
		}
	}
	return 0;
}

cJSON *cJSONUtils_GetPointer(cJSON *object,const char *pointer)
{
	while (*pointer++=='/' && object)
	{
		if (object->type==cJSON_Array)
		{
			int which=0; while (*pointer>='0' && *pointer<='9') which=(10*which) + *pointer++ - '0';
			if (*pointer && *pointer!='/') return 0;
			object=cJSON_GetArrayItem(object,which);
		}
		else if (object->type==cJSON_Object)
		{
			object=object->child;	while (object && cJSONUtils_Pstrcasecmp(object->string,pointer)) object=object->next;	/* GetObjectItem. */
			while (*pointer && *pointer!='/') pointer++;
		}
		else return 0;
	}
	return object;
}

/* JSON Patch implementation. */
static void cJSONUtils_InplaceDecodePointerString(char *string)
{
	char *s2=string;
	for (;*string;s2++,string++) *s2=(*string!='~')?(*string):((*(++string)=='0')?'~':'/');
	*s2=0;
}

static cJSON *cJSONUtils_PatchDetach(cJSON *object,const char *path)
{
	char *parentptr=0,*childptr=0;cJSON *parent=0,*ret=0;

	parentptr=strdup(path);	childptr=strrchr(parentptr,'/');	if (childptr) *childptr++=0;
	parent=cJSONUtils_GetPointer(object,parentptr);
	cJSONUtils_InplaceDecodePointerString(childptr);

	if (!parent) ret=0;	/* Couldn't find object to remove child from. */
	else if (parent->type==cJSON_Array)		ret=cJSON_DetachItemFromArray(parent,atoi(childptr));
	else if (parent->type==cJSON_Object)	ret=cJSON_DetachItemFromObject(parent,childptr);
	free(parentptr);
	return ret;
}

static int cJSONUtils_Compare(cJSON *a,cJSON *b)
{
	if (a->type!=b->type)	return -1;	/* mismatched type. */
	switch (a->type)
	{
	case cJSON_Number:	return (a->valueint!=b->valueint || a->valuedouble!=b->valuedouble)?-2:0;	/* numeric mismatch. */
	case cJSON_String:	return (strcmp(a->valuestring,b->valuestring)!=0)?-3:0;						/* string mismatch. */
	case cJSON_Array:	for (a=a->child,b=b->child;a && b;a=a->next,b=b->next)	{int err=cJSONUtils_Compare(a,b);if (err) return err;}
						return (a || b)?-4:0;	/* array size mismatch. */
	case cJSON_Object:
						cJSONUtils_SortObject(a);
						cJSONUtils_SortObject(b);
						a=a->child,b=b->child;
						while (a && b)
						{
							int err;
							if (cJSONUtils_strcasecmp(a->string,b->string))	return -6;	/* missing member */
							err=cJSONUtils_Compare(a,b);if (err) return err;
							a=a->next,b=b->next;
						}
						return (a || b)?-5:0;	/* object length mismatch */

	default:			break;
	}
	return 0;
}

static int cJSONUtils_ApplyPatch(cJSON *object,cJSON *patch)
{
	cJSON *op=0,*path=0,*value=0,*parent=0;int opcode=0;char *parentptr=0,*childptr=0;

	op=cJSON_GetObjectItem(patch,"op");
	path=cJSON_GetObjectItem(patch,"path");
	if (!op || !path) return 2;	/* malformed patch. */

	if		(!strcmp(op->valuestring,"add"))	opcode=0;
	else if (!strcmp(op->valuestring,"remove")) opcode=1;
	else if (!strcmp(op->valuestring,"replace"))opcode=2;
	else if (!strcmp(op->valuestring,"move"))	opcode=3;
	else if (!strcmp(op->valuestring,"copy"))	opcode=4;
	else if (!strcmp(op->valuestring,"test"))	return cJSONUtils_Compare(cJSONUtils_GetPointer(object,path->valuestring),cJSON_GetObjectItem(patch,"value"));
	else return 3; /* unknown opcode. */

	if (opcode==1 || opcode==2)	/* Remove/Replace */
	{
		cJSON_Delete(cJSONUtils_PatchDetach(object,path->valuestring));	/* Get rid of old. */
		if (opcode==1) return 0;	/* For Remove, this is job done. */
	}

	if (opcode==3 || opcode==4)	/* Copy/Move uses "from". */
	{
		cJSON *from=cJSON_GetObjectItem(patch,"from");	if (!from) return 4; /* missing "from" for copy/move. */

		if (opcode==3) value=cJSONUtils_PatchDetach(object,from->valuestring);
		if (opcode==4) value=cJSONUtils_GetPointer(object,from->valuestring);
		if (!value) return 5; /* missing "from" for copy/move. */
		if (opcode==4) value=cJSON_Duplicate(value,1);
		if (!value) return 6; /* out of memory for copy/move. */
	}
	else	/* Add/Replace uses "value". */
	{
		value=cJSON_GetObjectItem(patch,"value");
		if (!value) return 7; /* missing "value" for add/replace. */
		value=cJSON_Duplicate(value,1);
		if (!value) return 8; /* out of memory for add/replace. */
	}
		
	/* Now, just add "value" to "path". */

	parentptr=strdup(path->valuestring);	childptr=strrchr(parentptr,'/');	if (childptr) *childptr++=0;
	parent=cJSONUtils_GetPointer(object,parentptr);
	cJSONUtils_InplaceDecodePointerString(childptr);

	/* add, remove, replace, move, copy, test. */
	if (!parent) {free(parentptr); cJSON_Delete(value); return 9;}	/* Couldn't find object to add to. */
	else if (parent->type==cJSON_Array)
	{
		if (!strcmp(childptr,"-"))	cJSON_AddItemToArray(parent,value);
		else						cJSON_InsertItemInArray(parent,atoi(childptr),value);
	}
	else if (parent->type==cJSON_Object)
	{
		cJSON_DeleteItemFromObject(parent,childptr);
		cJSON_AddItemToObject(parent,childptr,value);
	}
	else
	{
		cJSON_Delete(value);
	}
	free(parentptr);
	return 0;
}


int cJSONUtils_ApplyPatches(cJSON *object,cJSON *patches)
{
	int err;
	if (patches->type!=cJSON_Array) return 1;	/* malformed patches. */
	if (patches) patches=patches->child;
	while (patches)
	{
		if ((err=cJSONUtils_ApplyPatch(object,patches))) return err;
		patches=patches->next;
	}
	return 0;
}

static void cJSONUtils_GeneratePatch(cJSON *patches,const char *op,const char *path,const char *suffix,cJSON *val)
{
	cJSON *patch=cJSON_CreateObject();
	cJSON_AddItemToObject(patch,"op",cJSON_CreateString(op));
	if (suffix)
	{
		char *newpath=(char*)malloc(strlen(path)+cJSONUtils_PointerEncodedstrlen(suffix)+2);
		cJSONUtils_PointerEncodedstrcpy(newpath+sprintf(newpath,"%s/",path),suffix);
		cJSON_AddItemToObject(patch,"path",cJSON_CreateString(newpath));
		free(newpath);
	}
	else	cJSON_AddItemToObject(patch,"path",cJSON_CreateString(path));
	if (val) cJSON_AddItemToObject(patch,"value",cJSON_Duplicate(val,1));
	cJSON_AddItemToArray(patches,patch);
}

void cJSONUtils_AddPatchToArray(cJSON *array,const char *op,const char *path,cJSON *val)	{cJSONUtils_GeneratePatch(array,op,path,0,val);}

static void cJSONUtils_CompareToPatch(cJSON *patches,const char *path,cJSON *from,cJSON *to)
{
	if (from->type!=to->type)	{cJSONUtils_GeneratePatch(patches,"replace",path,0,to);	return;	}
	
	switch (from->type)
	{
	case cJSON_Number:	
		if (from->valueint!=to->valueint || from->valuedouble!=to->valuedouble)
			cJSONUtils_GeneratePatch(patches,"replace",path,0,to);
		return;
						
	case cJSON_String:	
		if (strcmp(from->valuestring,to->valuestring)!=0)
			cJSONUtils_GeneratePatch(patches,"replace",path,0,to);
		return;

	case cJSON_Array:
	{
		int c;char *newpath=(char*)malloc(strlen(path)+23);	/* Allow space for 64bit int. */
		for (c=0,from=from->child,to=to->child;from && to;from=from->next,to=to->next,c++){
										sprintf(newpath,"%s/%d",path,c);	cJSONUtils_CompareToPatch(patches,newpath,from,to);
		}
		for (;from;from=from->next,c++)	{sprintf(newpath,"%d",c);	cJSONUtils_GeneratePatch(patches,"remove",path,newpath,0);	}
		for (;to;to=to->next,c++)		cJSONUtils_GeneratePatch(patches,"add",path,"-",to);
		free(newpath);
		return;
	}

	case cJSON_Object:
	{
		cJSON *a,*b;
		cJSONUtils_SortObject(from);
		cJSONUtils_SortObject(to);
		
		a=from->child,b=to->child;
		while (a || b)
		{
			int diff=(!a)?1:(!b)?-1:cJSONUtils_strcasecmp(a->string,b->string);
			if (!diff)
			{
				char *newpath=(char*)malloc(strlen(path)+cJSONUtils_PointerEncodedstrlen(a->string)+2);
				cJSONUtils_PointerEncodedstrcpy(newpath+sprintf(newpath,"%s/",path),a->string);
				cJSONUtils_CompareToPatch(patches,newpath,a,b);
				free(newpath);
				a=a->next;
				b=b->next;
			}
			else if (diff<0)	{cJSONUtils_GeneratePatch(patches,"remove",path,a->string,0);	a=a->next;}
			else				{cJSONUtils_GeneratePatch(patches,"add",path,b->string,b);		b=b->next;}
		}
		return;
	}

	default:			break;
	}
}


cJSON* cJSONUtils_GeneratePatches(cJSON *from,cJSON *to)
{
	cJSON *patches=cJSON_CreateArray();	
	cJSONUtils_CompareToPatch(patches,"",from,to);
	return patches;
}


static cJSON *cJSONUtils_SortList(cJSON *list)
{
	cJSON *first=list,*second=list,*ptr=list;

	if (!list || !list->next) return list;	/* One entry is sorted already. */
	
	while (ptr && ptr->next && cJSONUtils_strcasecmp(ptr->string,ptr->next->string)<0) ptr=ptr->next;	/* Test for list sorted. */
	if (!ptr || !ptr->next) return list;	/* Leave sorted lists unmodified. */
	ptr=list;

	while (ptr) {second=second->next;ptr=ptr->next;if (ptr) ptr=ptr->next;}	/* Walk two pointers to find the middle. */
	if (second && second->prev) second->prev->next=0;	/* Split the lists */

	first=cJSONUtils_SortList(first);	/* Recursively sort the sub-lists. */
	second=cJSONUtils_SortList(second);
	list=ptr=0;

	while (first && second)	/* Merge the sub-lists */
	{		
		if (cJSONUtils_strcasecmp(first->string,second->string)<0)
		{
			if (!list) list=ptr=first;
			else	{ptr->next=first;first->prev=ptr;ptr=first;}
			first=first->next;
		}
		else 
		{
			if (!list) list=ptr=second;
			else	{ptr->next=second;second->prev=ptr;ptr=second;}
			second=second->next;
		}
	}
	if (first)	{	if (!list) return first;	ptr->next=first;	first->prev=ptr;	}	/* Append any tails. */
	if (second)	{	if (!list) return second;	ptr->next=second;	second->prev=ptr;	}

	return list;
}

void cJSONUtils_SortObject(cJSON *object)	{object->child=cJSONUtils_SortList(object->child);}


int json_test()
{

/*
    //创建一个空的文档（对象）
     cJSON *json = cJSON_CreateObject();

     //向文档中增加一个键值对{"name":"王大锤"}
     cJSON_AddItemToObject(json,"name",cJSON_CreateString("王大锤"));
     //向文档中添加一个键值对
     //cJSON_AddItemToObject(json,"age",cJSON_CreateNumber(29));
     cJSON_AddNumberToObject(json,"age",29);

     cJSON *array = NULL;
     cJSON_AddItemToObject(json,"love",array=cJSON_CreateArray());
     cJSON_AddItemToArray(array,cJSON_CreateString("LOL"));
     cJSON_AddItemToArray(array,cJSON_CreateString("NBA"));
     cJSON_AddItemToArray(array,cJSON_CreateString("Go shopping"));

     cJSON_AddNumberToObject(json,"score",59);
     cJSON_AddStringToObject(json,"address","beijing");
*/

     cJSON *pJsonRoot = NULL;
     pJsonRoot = cJSON_CreateObject();
     if(NULL == pJsonRoot)
     {
         return -1;
     }
     cJSON_AddStringToObject(pJsonRoot,"sayhi","hello world!");
     cJSON_AddBoolToObject(pJsonRoot,"I`m a man",1);
     cJSON *pJsonChild = cJSON_CreateObject();
     if(NULL == pJsonChild)
     {
         cJSON_Delete(pJsonRoot);
         return -1;
     }
     cJSON_AddStringToObject(pJsonChild, "name", "my name is fengdj");
     cJSON_AddNumberToObject(pJsonChild, "my age", 30);
     cJSON * pJsonSubChild = cJSON_CreateObject();//新建一个JSON项目

     if( NULL == pJsonSubChild)
     {
         cJSON_Delete(pJsonRoot);
         cJSON_Delete(pJsonChild);
         return -1;
     }
     cJSON_AddStringToObject(pJsonSubChild, "job", "I`m a programmer");
     cJSON_AddFalseToObject(pJsonSubChild, "baby");
     cJSON_AddItemToObject(pJsonChild, "sub child item", pJsonSubChild);//把pJsonSubChild项目添加到pJsonChild上。
     cJSON_AddItemToObject(pJsonRoot, "child item", pJsonChild);//把pJsonChild项目添加到Root项 pJsonRoot

    // char * lpJsonstr = NULL;

     //将json结构格式化到缓冲区
     char *buf = cJSON_Print(pJsonRoot);
     //打开文件写入json内容
     FILE *fp = fopen("create.json","w");
     fwrite(buf,strlen(buf),1,fp);

     cJSON* pJson = cJSON_Parse(buf);
     if(NULL == pJson)
     {
         return -1;
     }
     if(pJson->type == cJSON_Object)
     {
         cJSON *pJson1 = cJSON_GetObjectItem(pJson,"sayhi");
         if(pJson1 == NULL)
         {
             return -1;
         }
         printf("%s\n",pJson1->valuestring);

            cJSON *pNext = pJson1->next;
            if(NULL == pNext)
            {
                return -1;
            }
            else {
                int ivalue = pNext->valueint;
                printf("+++%d \n",ivalue);
                cJSON *pPre = pNext->prev;
                if(NULL != pPre)
                {
                    printf("++%s \n",pPre->valuestring);
                }
            }
      }



     free(buf);
     fclose(fp);

#define test01
#ifdef test01
    //先创建空对象
        cJSON *json = cJSON_CreateObject();
        //在对象上添加键值对
        cJSON_AddStringToObject(json,"country","china");
        //添加数组
        cJSON *array = NULL;
        cJSON_AddItemToObject(json,"stars",array=cJSON_CreateArray());
        //在数组上添加对象
        cJSON *obj = NULL;
        cJSON_AddItemToArray(array,obj=cJSON_CreateObject());
        cJSON_AddItemToObject(obj,"name",cJSON_CreateString("Faye"));
        cJSON_AddStringToObject(obj,"address","beijing");
        //在对象上添加键值对
        cJSON_AddItemToArray(array,obj=cJSON_CreateObject());
        cJSON_AddItemToObject(obj,"name",cJSON_CreateString("andy"));
        cJSON_AddItemToObject(obj,"address",cJSON_CreateString("HK"));

        cJSON_AddItemToArray(array,obj=cJSON_CreateObject());
        cJSON_AddStringToObject(obj,"name","eddie");
        cJSON_AddStringToObject(obj,"address","TaiWan");

        //清理工作
        FILE *fp1 = fopen("mytest.json","w");
        buf = cJSON_Print(json);
        fwrite(buf,strlen(buf),1,fp1);
        fclose(fp1);
#endif

#define simpletest
#ifdef simpletest
    char *data = "{\"love\":[\"match\",\"Go shopping \"]}";
    json = cJSON_Parse(data);
    char *json_data = NULL;
    printf("data :%s \n",json_data = cJSON_Print(json));
    FILE *fp2 = fopen("test.json","w");
    fwrite(json_data,strlen(json_data),1,fp2);
    fclose(fp2);
    free(json_data);
#endif


    cJSON_Delete(pJsonRoot);
    return 0;
}

int TLS_json() {
	char *buf = NULL;
	cJSON *pJsonRoot = cJSON_CreateObject();
	//在对象上添加键值对

	cJSON *pJsonChildtun = cJSON_CreateObject();
	if (NULL == pJsonChildtun)
	{
		cJSON_Delete(pJsonRoot);
		return -1;
	}	
	cJSON * pJsonSubChild = cJSON_CreateObject();//新建一个JSON项目

	if (NULL == pJsonSubChild)
	{
		cJSON_Delete(pJsonRoot);
		cJSON_Delete(pJsonChildtun);
		return -1;
	}
	
	cJSON_AddStringToObject(pJsonSubChild, "srcport", "8080");
	cJSON_AddStringToObject(pJsonSubChild, "type", "tcp");
	cJSON_AddStringToObject(pJsonSubChild, "destip", "192.168.1.123");
	cJSON_AddStringToObject(pJsonSubChild, "destport", "6060");
	cJSON_AddItemToObject(pJsonChildtun, "tunnelA", pJsonSubChild);//把pJsonSubChild项目添加到pJsonChild上。
	cJSON_AddItemToObject(pJsonRoot, "tunconfig", pJsonChildtun);//把pJsonChild项目添加到Root项 pJsonRoot
	

	cJSON *pJsonChildapp = cJSON_CreateObject();
	if (NULL == pJsonChildapp)
	{
		cJSON_Delete(pJsonRoot);
		return -1;
	}
	cJSON * pJsonSubChildapp = cJSON_CreateObject();//新建一个JSON项目

	if (NULL == pJsonSubChild)
	{
		cJSON_Delete(pJsonRoot);
		cJSON_Delete(pJsonChildapp);
		return -1;
	}
	cJSON_AddStringToObject(pJsonSubChildapp, "name", "app01");
	cJSON_AddStringToObject(pJsonSubChildapp, "certdir", "app01path");

	cJSON *array = cJSON_CreateArray();
	cJSON_AddItemToObject(pJsonSubChildapp, "apptun", array );
	cJSON_AddNumberToObject(array, "apptun", 1);
	cJSON_AddNumberToObject(array, "apptun", 2);
	cJSON_AddNumberToObject(array, "apptun", 3);
	cJSON_AddNumberToObject(array, "apptun", 4);
	cJSON_AddStringToObject(array, "apptun", "testarray");

	//cJSON_AddItemToArray(array, "number");

	cJSON_AddItemToObject(pJsonChildapp, "APP01", pJsonSubChildapp);//把pJsonSubChild项目添加到pJsonChild上。
	cJSON_AddItemToObject(pJsonRoot, "appconfig", pJsonChildapp);//把pJsonChild项目添加到Root项 pJsonRoot

	//清理工作
	FILE *fp1 = fopen("tls_example.json", "w");
	buf = cJSON_Print(pJsonRoot);
	fwrite(buf, strlen(buf), 1, fp1);
	fclose(fp1);
	cJSON_Delete(pJsonRoot);
	cJSON* cjson = cJSON_Parse(buf);
	printf("%s \n", cJSON_GetObjectItem(cjson, "appconfig")->string);


	return 0;
}