#ifndef ELEMENTLISTUTIL_H
#define ELEMENTLISTUTIL_H

template <typename ListType, typename ItemType>
int generateNomber(const QList<ListType> &itemList)
{
	if(itemList.isEmpty()) return 0;
	if(!itemList.at(0)) return 0;

	int magicMaxNomber = -999;
	int maxNomber = magicMaxNomber;
	foreach(ListType item, itemList)
	{
		ItemType baseItem = dynamic_cast<ItemType>(item);
		if(!baseItem) continue;
		if(maxNomber < baseItem->nomber()) maxNomber = baseItem->nomber();
	}

	if(maxNomber == magicMaxNomber) maxNomber = -1;
	return ++maxNomber;
}

template<typename ElementType, typename SourceListType>
QList<ElementType> getElements(const QList<SourceListType> &sourceList, const QString &typeID = QString())
{
	QList<ElementType> elementList;
	foreach(SourceListType item, sourceList)
	{
		ElementType element = dynamic_cast<ElementType>(item);
		if(element && (typeID.isEmpty() || (element->typeID() == typeID))) elementList << element;
	}
	return elementList;
}

#endif // ELEMENTLISTUTIL_H