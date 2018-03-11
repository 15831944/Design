#pragma once
//材料类
//[XXT]最好用等宽字体Consolas显示，方便对位
//[XXT]显示出行号，方便别人定位
//[XXT]一般不用命名空间，需要时直接加范围符

class Material
{
public://通用数据
	enum E_MaterialType//[XXT]枚举类型名称最好加E_的标识
	{
		E_MT_MATERIAL,//[XXT]枚举内容最好写的细些防止与其他人冲突，如E_枚举类型简写_...
		E_MT_MASONRY,//砌体
		E_MT_CONCRETE,//砼
		E_MT_REINFORCEMENT,//普通钢筋
		E_MT_TENSION,//预应力钢束
		E_MT_STEEL//钢材
	};

public:
	Material();
	virtual ~Material();

	virtual E_MaterialType getType(){return E_MaterialType::E_MT_MATERIAL;}
	void setName(double name);//设置材料名称
	virtual void updateInfo() = 0;//更新数据
	double getName() const;//[XXT]不改变成员变量的函数最好加上const
	double get_E() const;
	double get_G() const;
	double get_ν() const;//[XXT]变量名最好只用英文+数字，不用希腊字母
	double get_αc() const;
	double get_γ() const;

protected:
	double name;//标号
	double E;//弹性模量
	double G;//剪切模量
	double ν;//泊松比
	double αc;//线膨胀系数
	double γ;//容重
};

//*------------------------------------*//

class Concrete : public Material
{
public:
	Concrete(){}//要弄一个不带参数的constructor，否则vector这种容器无法用于对应的类
	explicit Concrete(double name);//[XXT]对于单参数的constructor来说，最好加explicit防止单一参数被自动转化为当前类
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
	double get_ε0() const;
	double get_εcu() const;
	double get_α1() const;
	double get_β1() const;

private:
	double fcuk;
	double fck;
	double ftk;
	double fc;
	double ft;
	double fcc;//素砼抗压
	double fct;//素砼抗拉
	double ε0;//砼达到fc时的压应变
	double εcu;//砼正截面极限压应变
	double α1;
	double β1;

	double calc_αc1() const;//计算fck用
	double calc_αc2() const;//计算fck用
	double calc_δ() const;//变异系数，计算ftk用
	double calc_ε0() const;
	double calc_εcu() const;
	double calc_α1() const;
	double calc_β1() const;
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