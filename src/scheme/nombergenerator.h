#ifndef NOMBERGENERATOR_H
#define NOMBERGENERATOR_H

template <typename ListType, typename ItemType>
int generateNomber(const QList<ListType> &itemList)
{
	if(itemList.isEmpty()) return 0;
	if(!itemList.at(0)) return 0;
	if(!dynamic_cast<ItemType>(itemList.at(0))) return 0;

	int maxNomber = dynamic_cast<ItemType>(itemList.at(0))->nomber();
	foreach(ListType item, itemList)
	{
		ItemType baseItem = dynamic_cast<ItemType>(item);
		if(!baseItem) continue;
		if(maxNomber < baseItem->nomber()) maxNomber = baseItem->nomber();
	}

	return ++maxNomber;
}

#endif // NOMBERGENERATOR_H