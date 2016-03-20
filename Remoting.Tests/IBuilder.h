#pragma once

class IBuilder
{
public:
	virtual IAcceptor* CreateAcceptor() = 0;
	virtual IConnector* CreateConnector() = 0;
	virtual ~IBuilder() {};
};
