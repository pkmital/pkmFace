/****************************************************************************
*						AAMLibrary
*			http://code.google.com/p/aam-library
* Copyright (c) 2008-2009 by GreatYao, all rights reserved.
****************************************************************************/

#include "AAM_Shape.h"

using namespace std;

AAM_Shape::AAM_Shape()
{
	resize(0);
}

AAM_Shape::AAM_Shape( const AAM_Shape &s )
{
    CopyData (s);
}

AAM_Shape::~AAM_Shape()
{
	clear();
}

void AAM_Shape::resize(int length)
{
	m_vPoint.resize(length);
}

void AAM_Shape::clear()
{
	m_vPoint.clear();
}

const CvPoint2D32f AAM_Shape::operator[] (int i)const
{  
	return m_vPoint[i];	
}

CvPoint2D32f& AAM_Shape::operator[] (int i)
{  
	return m_vPoint[i];	
}
	
const int AAM_Shape::NPoints()const
{ 
	return	m_vPoint.size(); 
}

AAM_Shape& AAM_Shape::operator=(const AAM_Shape &s)
{
    CopyData (s);
    return *this;
}


const double AAM_Shape::GetWidth()const
{  
	return MaxX()-MinX();    
}

const double AAM_Shape::GetHeight()const
{ 
	return MaxY()-MinY();	
}

AAM_Shape& AAM_Shape::operator=(double value)
{
    for (int i = 0, size = m_vPoint.size(); i < size; i++)
    {
		m_vPoint[i].x = value;
        m_vPoint[i].y = value;
	}   
	return *this;   
}

AAM_Shape AAM_Shape::operator+(const AAM_Shape &s)const
{    
//    return AAM_Shape(*this) += s;
	
	AAM_Shape res(*this);
    for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        res.m_vPoint[i].x += s.m_vPoint[i].x;
        res.m_vPoint[i].y += s.m_vPoint[i].y;
    }
    return res;
	
}

AAM_Shape& AAM_Shape::operator+=(const AAM_Shape &s)
{
    for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        m_vPoint[i].x += s.m_vPoint[i].x;
        m_vPoint[i].y += s.m_vPoint[i].y;
    }
    return *this;
}

AAM_Shape AAM_Shape::operator-(const AAM_Shape &s)const
{    
//	return AAM_Shape(*this) -= s;
	
    AAM_Shape res(*this);
    for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        res.m_vPoint[i].x -= s.m_vPoint[i].x;
        res.m_vPoint[i].y -= s.m_vPoint[i].y;
    }
    return res;
}

AAM_Shape& AAM_Shape::operator-=(const AAM_Shape &s)
{
    for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        m_vPoint[i].x -= s.m_vPoint[i].x;
        m_vPoint[i].y -= s.m_vPoint[i].y;
    }
    return *this;
}

AAM_Shape AAM_Shape::operator*(double value)const
{
//	return AAM_Shape(*this) *= value;
	
    AAM_Shape res(*this);
    for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        res.m_vPoint[i].x *= value;
        res.m_vPoint[i].y *= value;
    }
    return res;
}

AAM_Shape& AAM_Shape::operator*=(double value)
{
    for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        m_vPoint[i].x *= value;
        m_vPoint[i].y *= value;
    }
    return *this;
}

double AAM_Shape::operator*(const AAM_Shape &s)const
{
    double result = 0.0;
    for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        result += m_vPoint[i].x * s.m_vPoint[i].x +
			m_vPoint[i].y * s.m_vPoint[i].y;
    }
    return result;
}

AAM_Shape AAM_Shape::operator/(double value)const
{
 //   return AAM_Shape(*this) /= value;

	assert (value != 0);

    AAM_Shape res(*this);
    for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        res.m_vPoint[i].x /= value;
        res.m_vPoint[i].y /= value;
    }
    return res;
}

AAM_Shape& AAM_Shape::operator/=(double value)
{
    assert (value != 0);

    for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        m_vPoint[i].x /= value;
        m_vPoint[i].y /= value;
    }
    return *this;
}

bool AAM_Shape::operator==(double value)
{
	for (int i = 0, size = m_vPoint.size(); i < size; i++)
	{
        if( fabs(m_vPoint[i].x - value) > 1e-6)
			return false;

		if( fabs(m_vPoint[i].y - value) > 1e-6)
			return false;
    }

	return true;
}
	
void AAM_Shape::CopyData(const AAM_Shape &s )
{
	if(m_vPoint.size() != s.m_vPoint.size())
		m_vPoint.resize(s.m_vPoint.size());
	m_vPoint = s.m_vPoint;
}

//----------------------------------------------------------------------
// Get Euclidean Norm of Shape
//----------------------------------------------------------------------
double AAM_Shape::GetNorm2()const
{
    double norm = 0.0;

    // Normalize the vector to unit length, using the 2-norm.
    for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {
        norm += pow(m_vPoint[i].x, 2);
        norm += pow(m_vPoint[i].y, 2);
    }
    norm = sqrt(norm);
    return norm;
}

//---------------------------------------------------------------------
// Get the center of Shape
//----------------------------------------------------------------------
void AAM_Shape::COG(double &x, double &y)const
{
    x = y = 0.0;

    for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {
        x += m_vPoint[i].x;
        y += m_vPoint[i].y;
    }
    x /= m_vPoint.size();
    y /= m_vPoint.size();

}

//---------------------------------------------------------------------
// Translate Shape so that its center of gravity is at the origin
//----------------------------------------------------------------------
void AAM_Shape::Centralize()
{
    double xSum, ySum;

	COG(xSum, ySum);

	Translate(-xSum, -ySum);

}

//---------------------------------------------------------------------
// Translate Shape using (x, y) 
//----------------------------------------------------------------------
void AAM_Shape::Translate(double x, double y)
{
    for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {
        m_vPoint[i].x += x;
        m_vPoint[i].y += y;
    }
}

//---------------------------------------------------------------------
// Scale Shape using s
//----------------------------------------------------------------------
void AAM_Shape::Scale(double s)
{
    for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {
        m_vPoint[i].x *= s;
        m_vPoint[i].y *= s;
    }
}

//----------------------------------------------------------------------
// Scale horizontal and vertical respectively
//----------------------------------------------------------------------
void AAM_Shape::ScaleXY(double sx, double sy)
{
    for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {
        m_vPoint[i].x *= sx;
        m_vPoint[i].y *= sy;
    }
}

double AAM_Shape::Normalize()
{
    double x,y;

    COG(x, y);

    Translate( -x, -y );

    double norm = GetNorm2();
	norm = (norm < 1e-10) ? 1 : norm; 

    Scale( 1./norm );

    return norm;
}

//----------------------------------------------------------------------
// Shape = (c00, c01; c10, c11) * Shape
//----------------------------------------------------------------------
void AAM_Shape::Transform(double c00, double c01, double c10, double c11)
{
	double x, y;
	for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {
        x = m_vPoint[i].x;
        y = m_vPoint[i].y;
        m_vPoint[i].x = c00*x+c01*y;
        m_vPoint[i].y = c10*x+c11*y;
    }

}

//----------------------------------------------------------------------
// Rotate Shape as theta
//----------------------------------------------------------------------
void AAM_Shape::Rotate(double theta)
{
    // set up transform matrix of rotation
    double c00 =  cos( theta );
    double c01 = -sin( theta );
    double c10 =  sin( theta );
    double c11 =  cos( theta );

	Transform(c00, c01, c10, c11);   
}

// ---------------------------------------------------------------------------
// Align Shape to to RefShape using a similarity transform.
// See also algorithm C.3 in Appendix C of [Cootes & Taylor, 2004]
// www.isbe.man.ac.uk/~bim/Models/app_models.pdf.
// returns the similarity transform matrix:
//      a -b  Tx
//      b  a  Ty
//      0  0  1
// Note BY YAO Wei: the Shape itself doesn't change
// ---------------------------------------------------------------------------
void AAM_Shape::AlignTransformation(const AAM_Shape &ref, 
		double &a, double &b, double &tx, double &ty)const
{

	double X1 = 0, Y1 = 0, X2 = 0, Y2 = 0, Z = 0, C1 = 0, C2 = 0;
	double W = m_vPoint.size();
	double x1, y1, x2, y2;
	
	for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {
		x1 = ref.m_vPoint[i].x;
        y1 = ref.m_vPoint[i].y;
		x2 = m_vPoint[i].x;
        y2 = m_vPoint[i].y;
		
		Z  += x2 * x2 + y2 * y2;
		X1 += x1;
		Y1 += y1;
		X2 += x2;
		Y2 += y2;
		C1 += x1 * x2 + y1 * y2;
		C2 += y1 * x2 - x1 * y2;
	}
	
	{
		double SolnA[] = {X2, -Y2, W, 0, Y2, X2, 0, W, Z, 0, X2, Y2, 0, Z, -Y2, X2};
		CvMat A = cvMat(4, 4, CV_64FC1, SolnA);
		double SolnB[] = {X1, Y1, C1, C2};
		CvMat B = cvMat(4, 1, CV_64FC1, SolnB);

		static CvMat* Soln = cvCreateMat(4, 1, CV_64FC1);
		cvSolve(&A, &B, Soln, CV_SVD);

		a	= CV_MAT_ELEM(*Soln, double, 0, 0);  b	= CV_MAT_ELEM(*Soln, double, 1, 0);
		tx	= CV_MAT_ELEM(*Soln, double, 2, 0);	 ty	= CV_MAT_ELEM(*Soln, double, 3, 0);
	}

	// Explained by YAO Wei, 2008.01.29.
	// It is equivalent as follows, but the former method of is more robust.
	/************************************************************************/
	/*		a	=	C1 / Z			b	=	C2 / Z							*/
	/*		tx	=	X1 / W			ty	=	Y1 / W							*/
	/************************************************************************/	
}

//----------------------------------------------------------------------
// The same as above, but the Shape itself will change
//----------------------------------------------------------------------
void AAM_Shape::AlignTo(const AAM_Shape &ref)
{
	double a, b, tx, ty;

	AlignTransformation(ref, a, b, tx, ty);
	
	TransformPose(a, b, tx, ty);
}

//----------------------------------------------------------------------
// Shape = (a, -b; b, a) * Shape + (tx, ty)
//----------------------------------------------------------------------
void AAM_Shape::TransformPose(double a, double b, double tx, double ty)
{
    Transform(a, -b, b, a);
	
	Translate( tx, ty );
}

const double AAM_Shape::MinX()const
{	
    double val, min = 1.7E+308;

    for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {	
        val = m_vPoint[i].x;
        min = val<min ? val : min;
    }
    return min;
}

const double AAM_Shape::MinY()const
{	
    double val, min = 1.7E+308;
    for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {	
        val = m_vPoint[i].y;
        min = val<min ? val : min;
    }
    return min;
}


const double AAM_Shape::MaxX()const
{	
    double val, max = -1.7E+308;
    for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {
        val = m_vPoint[i].x;
        max = val>max ? val : max;
    }
    return max;
}


const double AAM_Shape::MaxY()const
{	
    double val, max = -1.7E+308;
    for(int i = 0, size = m_vPoint.size(); i < size; i++)
    {
        val = m_vPoint[i].y;
        max = val>max ? val : max;
    }
    return max;
}
	
void AAM_Shape::Mat2Point(const CvMat* res)
{
    int nPoints = res->cols / 2;
	double *B = (double*)(res->data.ptr+res->step*0);
	if((int)m_vPoint.size() != nPoints)		resize(nPoints);
	for (int i = 0; i < nPoints ; i++)
    {
        m_vPoint[i].x = B[2*i];
        m_vPoint[i].y = B[2*i+1];
	}
}

void AAM_Shape::Point2Mat(CvMat* res)const
{
	int nPoints = res->cols / 2;
	double *B = (double*)(res->data.ptr+res->step*0);
	for (int i = 0; i < nPoints ; i++)
    {
        B[2*i] = m_vPoint[i].x;
        B[2*i+1] = m_vPoint[i].y;
	}
}

bool AAM_Shape::ReadAnnotations(const std::string &filename)
{
	int n = filename.length();
	if(filename.at(n-4) == '.' &&
		(filename.at(n-3) == 'A' || filename.at(n-3) == 'a') &&
		(filename.at(n-2) == 'S' || filename.at(n-2) == 's') &&
		(filename.at(n-1) == 'F' || filename.at(n-1) == 'f'))
	{
		ReadASF(filename);
		return false;
	}
	else if(filename.at(n-4) == '.' &&
		(filename.at(n-3) == 'P' || filename.at(n-3) == 'p') &&
		(filename.at(n-2) == 'T' || filename.at(n-2) == 't') &&
		(filename.at(n-1) == 'S' || filename.at(n-1) == 's'))
	{
		ReadPTS(filename);
		return true;
	}
	else 
	{
		AAM_FormatMSG("Unsupported shape file format(%s) !", filename.c_str()+n-3);
		AAM_ERROR(errmsg);
	}
}

void AAM_Shape::ReadASF(const std::string &filename)
{
    FILE* f = fopen(filename.c_str(), "r");
	if(f == NULL)
	{
		AAM_FormatMSG("CANNOT open asf file %s!\n", filename.c_str());
		AAM_ERROR(errmsg);
	}
	
    char temp[1024];
	int i = 0;
		
	while(fgets(temp, 1024, f))
	{
		if(temp[0] == '#' || temp[0] == '\r' || temp[0] == '\n') continue;
		i = 0;
		while(temp[i] == ' ') i++;
		if(temp[i] == '#') continue;
		else if(temp[i] != '#')
			break;
	}
	
	int n;
	sscanf(temp+i, "%d", &n);
	this->resize(n);
	
    while(fgets(temp, 1024, f))
	{
		if(temp[0] == '#' || temp[0] == '\r' || temp[0] == '\n') continue;
		i = 0;
		while(temp[i] == ' ') i++;
		if(temp[i] == '#') continue;
		else if(temp[i] != '#')
			break;
	}
	
	i = 0;
	double x, y;
	do{
		sscanf(temp, "%*d%*d%lf%lf", &x, &y);
		m_vPoint[i].x = x;
		m_vPoint[i].y = y;
		fgets(temp, 1024, f);
		i ++;
	}while(i < n);
	fclose(f);
}

void AAM_Shape::ReadPTS(const std::string& filename)
{
	FILE* f = fopen(filename.c_str(), "r");
	if(f == NULL)
	{
		AAM_FormatMSG("CANNOT open asf file %s!\n", filename.c_str());
		AAM_ERROR(errmsg);
	}
/*
    char temp[1024];
	int i, n;
	double x, y;

	fgets(temp, 1024, f);
	fgets(temp, 1024, f);
	sscanf(temp, "%*s %d", &n);
	this->resize(n);
	fgets(temp, 1024, f);
	
	i = 0;
	do{
		fscanf(f, "%lf%lf", &x, &y);
		m_vPoint[i].x = x;
		m_vPoint[i].y = y;
		i ++;
	}while(i < n);
	fclose(f);
 */
	int n = 0;
	fscanf(f, "%*s %d", &n);	// numpoints 68
	this->resize(n);
	int i = 0;
	float x,y;
	do {
		fscanf(f, "%f %f", &x, &y);
		m_vPoint[i].x = x;
		m_vPoint[i].y = y;
		i++;
	} while(i < n);
	fclose(f);
	
}

void AAM_Shape::Read(std::istream& is)
{
	for (int i = 0, nPoints = NPoints(); i < nPoints ; i++)
		is >> m_vPoint[i].x >> m_vPoint[i].y;
}

void AAM_Shape::Write(std::ostream& os)
{
	for (int i = 0, nPoints = NPoints(); i < nPoints ; i++)
		os << m_vPoint[i].x << " " << m_vPoint[i].y << " ";
	os << std::endl;
}
