#include <stdio.h>

#include <QDebug>
#include <QString>

#include "filter.h"
#include "factory.h"

QString apply(QString text, const QList<Filter *> &filters)
{
	for (Filter *filter : filters) {
		if (filter != nullptr) {
			text = filter->process(text);
		}
	}
	return text;
}

Filter * new_Reverse(){
	return new Reverse;
}

int main(void)
{
	// Je konstruktor factory metoda?
//	Filter *filter = new Reverse;

	// 1. Co je to factory metoda? Ako by sme ju mohli zadefinovat?
//	Filter *filter = new_Reverse();

	// 2. Ako staticka funkcia triedy
//	auto filter = Filter::create("REverse");

	// 3. Co ak mame viacero filtrov? Ako ich vytvarat efektivne? (1 call fun)
	// konstruktor, tepmlate, abstraktna trieda, ine?
	Filter *filter = Factory::create("Reverse");

//	qDebug() << filter->process("Some text");

	// 4. Implementacia pomocou Factory (zdruzenie viacerych FM)
//	auto filters = Factory::create({"Substitution", "Reverse"});

//	qDebug() << apply("Some text", filters);

	// 5. Ukazka ManuLab ...

    return 0;
}
