#if defined AVE_DB_CONN_INC
    #endinput
#endif
#define AVE_DB_CONN_INC

#include <a_mysql>

new MySQL:g_DatabaseHandle = MYSQL_INVALID_HANDLE;

// ====== Database_IsConnected ======
stock bool:Database_IsConnected()
{
    return g_DatabaseHandle != MYSQL_INVALID_HANDLE;
}

// ====== Database_Connect ======
stock Database_Connect()
{
    new MySQLOpt:connectionOptions = mysql_init_options();
    new errorMessage[144];

    mysql_set_option(connectionOptions, AUTO_RECONNECT, true);
    mysql_set_option(connectionOptions, POOL_SIZE, MYSQL_POOL_SIZE);

    g_DatabaseHandle = mysql_connect(
        MYSQL_HOST,
        MYSQL_USER,
        MYSQL_PASSWORD,
        MYSQL_DATABASE,
        connectionOptions
    );

    if (g_DatabaseHandle == MYSQL_INVALID_HANDLE || mysql_errno(g_DatabaseHandle) != 0)
    {
        mysql_error(errorMessage, sizeof(errorMessage), g_DatabaseHandle);
        printf(
            "[AvenueRoleplay] Failed to connect to MySQL. errno=%d error=%s",
            mysql_errno(g_DatabaseHandle),
            errorMessage
        );
        return 0;
    }

    print("[AvenueRoleplay] MySQL connection established.");
    return 1;
}

// ====== Database_Disconnect ======
stock Database_Disconnect()
{
    if (!Database_IsConnected())
    {
        return 1;
    }

    mysql_close(g_DatabaseHandle);
    g_DatabaseHandle = MYSQL_INVALID_HANDLE;
    print("[AvenueRoleplay] MySQL connection closed.");
    return 1;
}

// ====== Database_GetHandle ======
stock MySQL:Database_GetHandle()
{
    return g_DatabaseHandle;
}
