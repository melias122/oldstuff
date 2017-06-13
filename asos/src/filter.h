#ifndef __filter_h
#define __filter_h

#include <QString>

class Filter
{
public:
	virtual ~Filter() {}
	virtual QString process(QString text) = 0;
};

class Frequency : public Filter
{
public:
	QString process(QString text) override { return frequency(text); }
private:
	QString frequency(QString text);
};

class Substitution : public Filter
{
public:
	QString process(QString text) override {}
};

class Reverse : public Filter
{
public:
	QString process(QString text) override {
		std::reverse(text.begin(), text.end());
		return text;
	}
};


#endif
