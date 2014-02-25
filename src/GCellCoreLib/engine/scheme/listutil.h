#ifndef LISTUTIL_H
#define LISTUTIL_H

namespace gccore {

template <typename ListType, typename ItemType>
int generateNomber(const QList<ListType> &itemList)
{
    if (itemList.isEmpty())
        return 0;
    if (itemList.at(0) == nullptr)
        return 0;

	int magicMaxNomber = -999;
	int maxNomber = magicMaxNomber;
    foreach (ListType item, itemList) {
		ItemType baseItem = dynamic_cast<ItemType>(item);
        if (baseItem == nullptr)
            continue;
        if (maxNomber < baseItem->nomber())
            maxNomber = baseItem->nomber();
	}

    if (maxNomber == magicMaxNomber)
        maxNomber = -1;

	return ++maxNomber;
}

}

#endif // LISTUTIL_H