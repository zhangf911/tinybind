#ifndef TINY_XML_BIND
#define TINY_XML_BIND

#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
//#define TIXML_USE_STL
#include "tinyxml/tinystr.h"
#include "tinyxml/tinystr.cpp"
#include "tinyxml/tinyxml.h"
#include "tinyxml/tinyxml.cpp"
#include "tinyxml/tinyxmlerror.cpp"
#include "tinyxml/tinyxmlparser.cpp"
#ifndef WIN32 
	//to solve "error: ‘stricmp’ was not declared in this scope"
	#define stricmp strcasecmp 
	#define strnicmp strncasecmp 
#endif

void TXB_attr_bind( TiXmlElement* xmle, bool from, bool* attr_content, const char* attr_name)
{
	if ( from) //from xml
	{
		if ( xmle->Attribute(attr_name))
		{
			*attr_content = (std::string(xmle->Attribute(attr_name)).compare("true")==0);
		}
	}
	else //to xml
	{
		xmle->SetAttribute( attr_name, *attr_content?"true":"false");
	}
}
template <class T>
void TXB_attr_bind_int( TiXmlElement* xmle, bool from, T* attr_content, const char* attr_name)
{
	if ( from) //from xml
	{
		if ( xmle->Attribute(attr_name))
			*attr_content = (T)strtol(xmle->Attribute(attr_name),0,0);
	}
	else //to xml
	{
		char buf[100];
		snprintf(buf,100,"%d",*attr_content);
		xmle->SetAttribute( attr_name, buf);
	}
}
void TXB_attr_bind( TiXmlElement* xmle, bool from, int* attr_content, const char* attr_name)
{ TXB_attr_bind_int<int>(xmle,from,attr_content,attr_name); }
void TXB_attr_bind( TiXmlElement* xmle, bool from, unsigned int* attr_content, const char* attr_name)
{ TXB_attr_bind_int<unsigned int>(xmle,from,attr_content,attr_name); }
void TXB_attr_bind( TiXmlElement* xmle, bool from, char* attr_content, const char* attr_name)
{ TXB_attr_bind_int<char>(xmle,from,attr_content,attr_name); }
void TXB_attr_bind( TiXmlElement* xmle, bool from, unsigned char* attr_content, const char* attr_name)
{ TXB_attr_bind_int<unsigned char>(xmle,from,attr_content,attr_name); }
void TXB_attr_bind( TiXmlElement* xmle, bool from, short* attr_content, const char* attr_name)
{ TXB_attr_bind_int<short>(xmle,from,attr_content,attr_name); }
void TXB_attr_bind( TiXmlElement* xmle, bool from, unsigned short* attr_content, const char* attr_name)
{ TXB_attr_bind_int<unsigned short>(xmle,from,attr_content,attr_name); }

template <class T>
void TXB_attr_bind_float( TiXmlElement* xmle, bool from, T* attr_content, const char* attr_name)
{
	if ( from) //from xml
	{
		if ( xmle->Attribute(attr_name))
			*attr_content = (T)strtod(xmle->Attribute(attr_name),0);
	}
	else //to xml
	{
		char buf[100];
		snprintf(buf,100,"%f",*attr_content);
		xmle->SetAttribute( attr_name, buf);
	}
}
void TXB_attr_bind( TiXmlElement* xmle, bool from, float* attr_content, const char* attr_name)
{ TXB_attr_bind_float<float>(xmle,from,attr_content,attr_name); }
void TXB_attr_bind( TiXmlElement* xmle, bool from, double* attr_content, const char* attr_name)
{ TXB_attr_bind_float<double>(xmle,from,attr_content,attr_name); }

void TXB_attr_bind( TiXmlElement* xmle, bool from, std::string* attr_content, const char* attr_name)
{
	if ( from) //from xml
	{
		if ( xmle->Attribute(attr_name))
			*attr_content = xmle->Attribute( attr_name);
	}
	else //to xml
	{
		if ( attr_content->length()>0)
			xmle->SetAttribute( attr_name, attr_content->c_str());
	}
}

void TXB_text_bind( TiXmlElement* xmle, bool from, std::string* attr_content)
{
	if ( from) //from xml
	{
		const TiXmlNode* child = xmle->FirstChild();
		if ( child ) {
			const TiXmlText* childText = child->ToText();
			if ( childText ) {
				*attr_content = childText->Value();
			}
		}
	}
	else //to xml
	{
		if ( attr_content->length()>0)
		{
			TiXmlText txt( attr_content->c_str());
			if( xmle->FirstChild())
				xmle->InsertBeforeChild( xmle->FirstChild(), txt);
			else
				xmle->InsertEndChild( txt);
		}
	}
}

template <class T>
void TXB_ele_bind( TiXmlElement* xmle, bool from, std::vector<T>* ANI, const char* tagname)
{
	if ( from) //from xml
	{
		for( TiXmlElement* child = xmle->FirstChildElement(tagname);
			child;
			child = child->NextSiblingElement(tagname) )
		{
			T ani;
			TXB_binding(&ani, child, 1);
			ANI->push_back(ani);
		}
	}
	else //to xml
	{
		for ( int i=0;
			i<ANI->size();
			i++)
		{
			T* ani = &ANI->at(i);
			TiXmlElement Xani(tagname);
			TXB_binding(ani, &Xani, 0);
			xmle->InsertEndChild( Xani);
		}
	}
}

#endif //TINY_XML_BIND
