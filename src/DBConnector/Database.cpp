/*
 * Database.cpp
 *
 *  Created on: Sep 8, 2013
 *      Author: brydev
 */

#include "Database.h"

using namespace sql;

namespace MedMon_DB {

	Database::Database()
	{
		driver = get_driver_instance();
	}

	void Database::openDBConnection(std::string * dbURL, std::string * user, std::string * pw)
	{
		sql::SQLString dbStr(*dbURL);
		sql::SQLString dbUsr(*user);
		sql::SQLString usrPw(*pw);

		conn.reset(driver->connect(dbStr, dbUsr, usrPw));
		conn->setSchema("medmon");
	}

	void Database::initLJTbl(std::string * ljTblName)
	{
		std::cout << "Initializing labjack database\n";
		std::string initTblQuery = "CALL spNewLJDeviceTbl(?);";
		pstmt = conn->prepareStatement(sql::SQLString(initTblQuery));
		pstmt->setString(1, sql::SQLString(*ljTblName));
		pstmt->execute();
		delete pstmt;
	}

	void Database::recordSensorReading(int sensorID, int portNumber, int caliInfo, double value, std::string * dbNm)
	{
		std::string insertStmt = "INSERT INTO " + *dbNm + " (PortID, SensorID, TimeStamp, Calibration, SensorReading) " +
				"VALUES(?,?,NOW(),?,?)";

		sql::SQLString _query(insertStmt);
		pstmt = conn->prepareStatement(_query);
		pstmt->setInt(1, portNumber);
		pstmt->setInt(2, sensorID);
		pstmt->setInt(3, caliInfo);
		pstmt->setDouble(4, value);

		pstmt->execute();
	}

	void Database::executeNonQuery(const std::string * cmd)
	{
		std::cout << "Non Query called";
		sql::SQLString _query(*cmd);
		pstmt = conn->prepareStatement(_query);
		pstmt->execute();
		delete pstmt;
	}

	std::auto_ptr<sql::ResultSet> Database::executeQuery(const std::string * cmd)
	{
		sql::SQLString _query(*cmd);
		pstmt = conn->prepareStatement(_query);
		std::auto_ptr<sql::ResultSet> res(pstmt->executeQuery());
		delete pstmt;
		return res;
	}
} /* namespace MedMon_DB */
