/*
//! This keeps track of doors based on their open/close sound position.
//! I'm not sure if this is the best way but I figure it has to have less of a performance impact
//! than constantly searching around each zombie for a building and checking each door etc like other mods do.
//! The catch is it only tracks doors which have been interacted with by players, but if anything maybe that's a good thing
//! for server FPS as zombies will ignore any doors which have had no player activity during the server's restart session.
*/
class ZenZombieDoorDataManager
{
	// Store & handle door data
	private ref array<ref ZenZombieDoorData> m_DoorHitData;

	void Init()
	{
		if (!m_DoorHitData)
			m_DoorHitData = new array<ref ZenZombieDoorData>;
	}

	int GetDoorHitCount(vector doorPos, bool increase = true)
	{
		ZenZombieDoorData doorData = GetDoorData(doorPos);

		if (!doorData)
		{
			doorData = new ZenZombieDoorData(doorPos, 0);
			m_DoorHitData.Insert(doorData);
		}

		if (increase)
			doorData.m_HitCount = doorData.m_HitCount + 1;

		return doorData.m_HitCount;
	}

	ZenZombieDoorData GetDoorData(vector doorPos)
	{
		for (int i = 0; i < m_DoorHitData.Count(); i++)
		{
			ZenZombieDoorData doorData = m_DoorHitData.Get(i);
			if (doorData.m_DoorPos == doorPos)
				return doorData;
		}

		return NULL;
	}

	void RemoveDoorAt(vector doorPos)
	{
		for (int i = m_DoorHitData.Count() - 1; i >= 0; i--)
		{
			if (m_DoorHitData.Get(i).m_DoorPos == doorPos)
			{
				m_DoorHitData.Remove(i);
				break;
			};
		}
	}

	void AddDoor(vector v)
	{
		ZenZombieDoorData existingDoor = GetDoorData(v);

		if (existingDoor == NULL)
		{
			m_DoorHitData.Insert(new ZenZombieDoorData(v, 0));
		}
		else
		{
			existingDoor.m_HitCount = 0;
		}
	}

	void Destroy()
	{
		for (int i = 0; i < m_DoorHitData.Count(); i++)
		{
			delete m_DoorHitData.Get(i);
		}

		delete m_DoorHitData;
	}
}

class ZenZombieDoorData
{
	// Define door data
	vector m_DoorPos;
	int m_HitCount;

	void ZenZombieDoorData(vector pos, int count)
	{
		m_DoorPos = pos;
		m_HitCount = count;
	}
}

ref ZenZombieDoorDataManager m_ZenZombieDoorDataManager;

static ZenZombieDoorDataManager GetZenZombieDoorManager()
{
	if (!m_ZenZombieDoorDataManager)
	{
		ZMPrint("[ZombieDoorData] Init");
		m_ZenZombieDoorDataManager = new ZenZombieDoorDataManager;
	}

	return m_ZenZombieDoorDataManager;
}