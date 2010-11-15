#include "vcard.h"
#include <gloox/vcard.h>
#include <QByteArray>

MagnormoCard::MagnormoCard()
{
}

MagnormoCard::MagnormoCard(const gloox::VCard *vCard)
{
	gloox::VCard::Photo p = vCard->photo();
	QByteArray pba(p.binval.c_str(), p.binval.size());
	photo.loadFromData(pba);
}


MagnormoCard::~MagnormoCard()
{
}
