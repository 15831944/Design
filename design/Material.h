#pragma once
//������
//[XXT]����õȿ�����Consolas��ʾ�������λ
//[XXT]��ʾ���кţ�������˶�λ
//[XXT]һ�㲻�������ռ䣬��Ҫʱֱ�Ӽӷ�Χ��

class Material
{
public://ͨ������
	enum E_MaterialType//[XXT]ö������������ü�E_�ı�ʶ
	{
		E_MT_MATERIAL,//[XXT]ö���������д��ϸЩ��ֹ�������˳�ͻ����E_ö�����ͼ�д_...
		E_MT_MASONRY,//����
		E_MT_CONCRETE,//��
		E_MT_REINFORCEMENT,//��ͨ�ֽ�
		E_MT_TENSION,//ԤӦ������
		E_MT_STEEL//�ֲ�
	};

public:
	Material();
	virtual ~Material();

	virtual E_MaterialType getType(){return E_MaterialType::E_MT_MATERIAL;}
	void setName(double name);//���ò�������
	virtual void updateInfo() = 0;//��������
	double getName() const;//[XXT]���ı��Ա�����ĺ�����ü���const
	double get_E() const;
	double get_G() const;
	double get_��() const;//[XXT]���������ֻ��Ӣ��+���֣�����ϣ����ĸ
	double get_��c() const;
	double get_��() const;

protected:
	double name;//���
	double E;//����ģ��
	double G;//����ģ��
	double ��;//���ɱ�
	double ��c;//������ϵ��
	double ��;//����
};

//*------------------------------------*//

class Concrete : public Material
{
public:
	Concrete(){}//ҪŪһ������������constructor������vector���������޷����ڶ�Ӧ����
	explicit Concrete(double name);//[XXT]���ڵ�������constructor��˵����ü�explicit��ֹ��һ�������Զ�ת��Ϊ��ǰ��
	virtual ~Concrete();

	virtual E_MaterialType getType(){return E_MaterialType::E_MT_CONCRETE;}
	virtual void updateInfo();
	double get_fcuk() const;
	double get_fck() const;
	double get_ftk() const;
	double get_fc() const;
	double get_ft() const;
	double get_fcc() const;
	double get_fct() const;
	double get_��0() const;
	double get_��cu() const;
	double get_��1() const;
	double get_��1() const;

private:
	double fcuk;
	double fck;
	double ftk;
	double fc;
	double ft;
	double fcc;//���ſ�ѹ
	double fct;//���ſ���
	double ��0;//�Ŵﵽfcʱ��ѹӦ��
	double ��cu;//�������漫��ѹӦ��
	double ��1;
	double ��1;

	double calc_��c1() const;//����fck��
	double calc_��c2() const;//����fck��
	double calc_��() const;//����ϵ��������ftk��
	double calc_��0() const;
	double calc_��cu() const;
	double calc_��1() const;
	double calc_��1() const;
};

//*------------------------------------*//

class Rebar : public Material
{
public:
	Rebar(){}
	explicit Rebar(double name);
	virtual ~Rebar();

	virtual E_MaterialType getType(){return E_MaterialType::E_MT_REINFORCEMENT;}
	virtual void updateInfo();
	double get_fy() const;
	double get_fy_c() const;
	double get_fyv() const;

private:
	double fy;
	double fy_c;
	double fyv;

	double calc_fy() const;
	double calc_fy_c() const;
	double calc_fyv() const;
};

//*------------------------------------*//

class Steel : public Material
{
public:
	Steel(){}
	explicit Steel(double name);
	virtual ~Steel();

	virtual E_MaterialType getType(){ return E_MaterialType::E_MT_STEEL; }
	virtual void updateInfo();
	double get_fy() const;
	double get_f(double t) const;
	double get_fv(double t) const;
	double get_fce(double t) const;

private:
	double fy;

	double calc_f(double t) const;
	double calc_fv(double t) const;
	double calc_fce(double t) const;

	int getThicknessStage(double t) const;
};