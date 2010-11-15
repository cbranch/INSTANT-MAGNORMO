#ifndef VCARD_H
#define VCARD_H

#include <QMetaType>
#include <QPixmap>

namespace gloox {
	class VCard;
}

class MagnormoCard
{
public:
	MagnormoCard();
	MagnormoCard(const gloox::VCard *vCard);
	~MagnormoCard();

	QPixmap photo;
};

Q_DECLARE_METATYPE(MagnormoCard)

#endif