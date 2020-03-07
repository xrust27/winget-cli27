// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.
#pragma once
#include "SQLiteWrapper.h"
#include <optional>
#include <string>
#include <string_view>
#include <vector>


namespace AppInstaller::Repository::Microsoft::Schema::V1_0
{
    namespace details
    {
        // Creates the table.
        void CreateOneToOneTable(SQLite::Connection& connection, std::string_view tableName, std::string_view valueName);

        // Selects the value from the table, returning the rowid if it exists.
        std::optional<SQLite::rowid_t> OneToOneTableSelectIdByValue(SQLite::Connection& connection, std::string_view tableName, std::string_view valueName, std::string_view value);

        // Selects the value from the table, returning the rowid if it exists.
        std::optional<std::string> OneToOneTableSelectValueById(SQLite::Connection& connection, std::string_view tableName, std::string_view valueName, SQLite::rowid_t id);

        // Gets all row ids from the table.
        std::vector<SQLite::rowid_t> OneToOneTableGetAllRowIds(SQLite::Connection& connection, std::string_view tableName, std::string_view valueName);

        // Ensures that the values exists in the table.
        SQLite::rowid_t OneToOneTableEnsureExists(SQLite::Connection& connection, std::string_view tableName, std::string_view valueName, std::string_view value);

        // Removes the given row by its rowid if it is no longer referenced.
        void OneToOneTableDeleteIfNotNeededById(SQLite::Connection& connection, std::string_view tableName, std::string_view valueName, SQLite::rowid_t id);

        // Determines if the table is empty.
        bool OneToOneTableIsEmpty(SQLite::Connection& connection, std::string_view tableName);
    }

    // A table that represents a value that is 1:1 with a primary entry.
    template <typename TableInfo>
    struct OneToOneTable
    {
        // The value type.
        using value_t = std::string;

        // The id type
        using id_t = SQLite::rowid_t;

        // Creates the table.
        static void Create(SQLite::Connection& connection)
        {
            details::CreateOneToOneTable(connection, TableInfo::TableName(), TableInfo::ValueName());
        }

        // The name of the table.
        static constexpr std::string_view TableName()
        {
            return TableInfo::TableName();
        }

        // The value name of the table.
        static constexpr std::string_view ValueName()
        {
            return TableInfo::ValueName();
        }

        // Selects the value from the table, returning the rowid if it exists.
        static std::optional<SQLite::rowid_t> SelectIdByValue(SQLite::Connection& connection, std::string_view value)
        {
            return details::OneToOneTableSelectIdByValue(connection, TableInfo::TableName(), TableInfo::ValueName(), value);
        }

        // Selects the value from the table, returning it if it exists.
        static std::optional<value_t> SelectValueById(SQLite::Connection& connection, id_t id)
        {
            return details::OneToOneTableSelectValueById(connection, TableInfo::TableName(), TableInfo::ValueName(), id);
        }

        // Gets all row ids from the table.
        static std::vector<SQLite::rowid_t> GetAllRowIds(SQLite::Connection& connection)
        {
            return details::OneToOneTableGetAllRowIds(connection, TableInfo::TableName(), TableInfo::ValueName());
        }

        // Ensures that the given value exists in the table, returning the rowid.
        static SQLite::rowid_t EnsureExists(SQLite::Connection& connection, std::string_view value)
        {
            return details::OneToOneTableEnsureExists(connection, TableInfo::TableName(), TableInfo::ValueName(), value);
        }

        // Removes the given row by its rowid if it is no longer referenced.
        static void DeleteIfNotNeededById(SQLite::Connection& connection, SQLite::rowid_t id)
        {
            return details::OneToOneTableDeleteIfNotNeededById(connection, TableInfo::TableName(), TableInfo::ValueName(), id);
        }

        // Removes data that is no longer needed for an index that is to be published.
        static void PrepareForPackaging(SQLite::Connection&)
        {
            // There is currently nothing to do for these tables.
        }

        // Determines if the table is empty.
        static bool IsEmpty(SQLite::Connection& connection)
        {
            return details::OneToOneTableIsEmpty(connection, TableInfo::TableName());
        }
    };
}