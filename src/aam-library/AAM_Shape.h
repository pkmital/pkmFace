/****************************************************************************
*						AAMLibrary
*			http://code.google.com/p/aam-library
* Copyright (c) 2008-2009 by GreatYao, all rights reserved.
****************************************************************************/

#ifndef AAM_SHAPE_H
#define AAM_SHAPE_H

#include "AAM_Config.h"

class AAM_EXPORTS AAM_Shape
{
public:
    // Constructors and Destructor
    AAM_Shape();
    AAM_Shape(const AAM_Shape &v);
    ~AAM_Shape();

	//access elements
	const CvPoint2D32f operator[] (int i)const;
	CvPoint2D32f& operator[] (int i);

	const int NPoints()const;

    // operators
    AAM_Shape&    operator=(const AAM_Shape &s);
    AAM_Shape&    operator=(double value);
    AAM_Shape     operator+(const AAM_Shape &s)const;
    AAM_Shape&    operator+=(const AAM_Shape &s);
    AAM_Shape     operator-(const AAM_Shape &s)const;
    AAM_Shape&    operator-=(const AAM_Shape &s);
    AAM_Shape     operator*(double value)const;
    AAM_Shape&    operator*=(double value);
    double        operator*(const AAM_Shape &s)const;
    AAM_Shape     operator/(double value)const;
    AAM_Shape&    operator/=(double value);
	bool		  operator==(double value);

    void    clear();
    void    resize(int length);

	void	Read(std::istream& is);
	void	Write(std::ostream& os);
	bool	ReadAnnotations(const std::string &filename);
	void    ReadASF(const std::string &filename);
	void	ReadPTS(const std::string &filename);

	const double  MinX()const;
    const double  MinY()const;
    const double  MaxX()const;
    const double  MaxY()const;
    const double  GetWidth()const;
	const double  GetHeight()const;

    // Transformations
    void    COG(double &x, double &y)const;
    void    Centralize();
    void    Translate(double x, double y);
    void    Scale(double s);
    void    Rotate(double theta);
	void    ScaleXY(double sx, double sy);
	double	Normalize();
	
	// Align the shapes to reference shape 
	//													[a -b Tx]
	// returns the similarity transform: T(a,b,tx,ty) = [b  a Ty]
	//													[0  0  1]
	void    AlignTransformation(const AAM_Shape &ref, 
			double &a, double &b, double &tx, double &ty)const;
    
	// Align the shapes to reference shape as above, but no returns
    void    AlignTo(const AAM_Shape &ref);
    
	// Transform Shape using similarity transform T(a,b,tx,ty)
	void    TransformPose(double a, double b, double tx, double ty);

	// Euclidean norm
	double  GetNorm2()const;

	// conversion between CvMat and AAM_Shape
	void    Mat2Point(const CvMat* res);
	void    Point2Mat(CvMat* res)const;

private:
	void    CopyData(const AAM_Shape &s);
	void    Transform(double c00, double c01, double c10, double c11);

private:
	// point data.
	std::vector<CvPoint2D32f> m_vPoint; 

};


#endif  // AAM_SHAPE_H

