#pragma once

enum E_Section
{
	E_S_SECTION,
	E_S_RECT_SECTION,
	E_S_CIRCLE_SECTION
};


class Section
{
public:
	Section();
	virtual ~Section();

	virtual E_Section getType(){return E_Section::E_S_SECTION;}
	void set_angle(double angle);
	double get_angle() const;

protected:
	double m_A;//Ãæ»ý
	double m_Ix;
	double m_Iy;
	double m_angle;//×ª½Ç

};

class RectSection : public Section
{
public:
	RectSection(double b, double h);
	virtual ~RectSection();

	virtual E_Section getType(){return E_Section::E_S_RECT_SECTION;}
	void set_b(double b);
	void set_h(double h);
	double get_b() const;
	double get_h() const;
private:
	double m_b;
	double m_h;
};

class CircleSection : public Section
{
public:
	CircleSection();
	virtual ~CircleSection();

	virtual E_Section getType(){return E_Section::E_S_CIRCLE_SECTION;}
	void set_d(double d);
	double get_d() const;
private:
	double m_d;
};