#include <stdio.h>

#include <QDebug>
#include <QString>

class Filter;
class Noop;

class Factory
{
private:
	using factory_method = Filter* (*)();

	template<typename filter>
	static Filter* new_filter() {
		return new filter;
	}

	static QHash<QString, factory_method> factories;

public:
	static Filter *create(QString filter)
	{
		if (factories.contains(filter)) {
			return factories[filter]();
		}
		return nullptr;
	}

	static QList<Filter *> create(QStringList filters)
	{
		QList<Filter *> list;
		for (const auto &filter : filters) {
			auto f = create(filter);
			if (f != nullptr)
				list << f;
		}
		return list;
	}
};

QHash<QString, Factory::factory_method> Factory::factories = {
	{"Noop", Factory::new_filter<Noop>}
//	{"Frequency", Factory::new_filter<Frequency>},
//	{"SubAB", Factory::new_filter<SubAB>}
};

class Filter
{
public:
	virtual ~Filter() {}
	virtual QString process(QString text) = 0;
};

class Noop : public Filter
{
public:
	QString process(QString text) override { return text; }
};

class Frequency : public Filter
{
public:
	QString process(QString text) override { return frequency(text); }
private:
	QString frequency(QString text);
};

class Creator
{
public:
	virtual ~Creator() {}
	virtual Filter *create() = 0;
};

class FrequencyCreator :public Creator
{
public:
	Filter *create() { return new Frequency; }
};

QString apply(QString text, const QList<Filter *> &filters)
{
	for (Filter *filter : filters) {
		if (filter != nullptr) {
			text = filter->process(text);
		}
	}
	return text;
}

int main(void)
{
	Filter *filter = new Noop;
	// 1. Co je to factory metoda? Ako by sme ju mohli zadefinovat?
//	auto filter = new_Noop();

	// 2. Ako staticka funkcia triedy
//	auto filter = Noop::create();

	// 3. Co ak mame viacero filtrov? Ako ich vytvarat efektivne? (1 call fun)
	// konstruktor, tepmlate, abstraktna trieda, ine?
//	auto filter = ?

	qDebug() << filter->process("Some text");

	// 4. Implementacia pomocou Factory (zdruzenie viacerych FM)
	auto filters = Factory::create({"Filter1", "Filter2"});

	qDebug() << apply("Some text", filters);

	// 5. Ukazka ManuLab ...

    return 0;
}
