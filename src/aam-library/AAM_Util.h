/****************************************************************************
*						AAMLibrary
*			http://code.google.com/p/aam-library
* Copyright (c) 2008-2009 by GreatYao, all rights reserved.
****************************************************************************/

#ifndef AAM_UTIL_H
#define AAM_UTIL_H

#include "AAM_Config.h"
#include "AAM_Shape.h"

class AAM_PAW;
class VJfacedetect;

class AAM_EXPORTS AAM_Common
{
public:
	static file_lists ScanNSortDirectory(const std::string &path, 
		const std::string &extension);

	// Is the current shape within the image boundary?
	static void CheckShape(CvMat* s, int w, int h);

	static void DrawPoints(IplImage* image, const AAM_Shape& Shape);

	static void DrawTriangles(IplImage* image, const AAM_Shape& Shape, 
		const std::vector<std::vector<int> >&tris);
	
	static void DrawAppearance(IplImage*image, const AAM_Shape& Shape,
		const CvMat* t, const AAM_PAW& paw, 
		const AAM_PAW& refpaw, int nPlane);
};

//virtual class for Active Appearance Model
class AAM_EXPORTS AAM
{
public:
	AAM();
	virtual ~AAM() = 0;

	virtual const int GetType()const = 0;

	// Build aam model
	virtual void Build(const file_lists& pts_files, 
		const file_lists& img_files, double scale = 1.0, int nPlane = 3) = 0;
	
	// Fit the image using aam 
	virtual void Fit(const IplImage* image, AAM_Shape& Shape, 
		int max_iter = 30, bool showprocess = false) = 0;

	// Set search parameters zero
	virtual void SetAllParamsZero() = 0;

	// Init search parameters 
	virtual void InitParams(const IplImage* image) = 0;

	// Draw the image according search result
	virtual void Draw(IplImage* image, const AAM_Shape& Shape, int type) = 0;

	// Read data from stream 
	virtual void Read(std::ifstream& is) = 0;

	// Write data to stream
	virtual void Write(std::ofstream& os) = 0;

	// Get Mean Shape of model
	virtual const AAM_Shape GetMeanShape()const = 0;
	virtual const AAM_Shape GetReferenceShape()const = 0;
	
	virtual void PrintAppearanceVector() = 0;
	virtual CvMat* GetAppearanceVector() = 0;
};

class AAM_EXPORTS AAM_Pyramid
{
public:
	AAM_Pyramid();
	~AAM_Pyramid();
	
	// Build Multi-Resolution Active Appearance Model
	void Build(const file_lists& pts_files, const file_lists& img_files,
		int type = TYPE_AAM_IC, int nLevel = 1, int nPlane = 3);

	// Doing image alignment
	void Fit(const IplImage* image, AAM_Shape& Shape, 
		int max_iter = 30, bool showprocess = false);

	// Build mapping relation between detect box and shape
	void BuildDetectMapping(const file_lists& pts_files, 
		const file_lists& img_files, VJfacedetect& facedetect,
		double refWidth = 100);

	// Init shape from the mapping  
	bool InitShapeFromDetBox(AAM_Shape& Shape, VJfacedetect& facedetect, 
		const IplImage* image);

	// Write aam to file
	void WriteModel(const std::string& filename);

	// Read aam from file
	void ReadModel(const std::string& filename);
	
	void ShowModes();

	// Draw the image according search result
	void Draw(IplImage* image, const AAM_Shape& Shape, int type);

	// Get Mean Shape of model
	const AAM_Shape GetMeanShape()const;
	
	void PrintAppearanceVector()
	{
		if(__model.size() > 0)
		{
			__model[0]->PrintAppearanceVector();
		}
	}
	
	CvMat* GetAppearanceVector()
	{
		if(__model.size() > 0)
		{
			return __model[0]->GetAppearanceVector();
		}
	}
	
private:
	std::vector<AAM*>	__model;
	AAM_Shape			__VJDetectShape;
	double				__referenceWidth;

};

#endif // AAM_UTIL_H
