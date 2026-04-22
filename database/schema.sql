CREATE TABLE IF NOT EXISTS `accounts` (
    `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `username` VARCHAR(24) NOT NULL,
    `password_hash` VARCHAR(128) NOT NULL,
    `admin_level` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `created_at` DATETIME NOT NULL,
    `updated_at` DATETIME NOT NULL,
    `last_login_at` DATETIME NULL DEFAULT NULL,
    PRIMARY KEY (`id`),
    UNIQUE KEY `uq_accounts_username` (`username`)
);

CREATE TABLE IF NOT EXISTS `characters` (
    `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `account_id` INT UNSIGNED NOT NULL,
    `name` VARCHAR(24) NOT NULL,
    `gender` TINYINT UNSIGNED NOT NULL,
    `skin` SMALLINT UNSIGNED NOT NULL,
    `pos_x` FLOAT NOT NULL,
    `pos_y` FLOAT NOT NULL,
    `pos_z` FLOAT NOT NULL,
    `pos_a` FLOAT NOT NULL,
    `interior` INT NOT NULL DEFAULT 0,
    `world` INT NOT NULL DEFAULT 0,
    `created_at` DATETIME NOT NULL,
    `updated_at` DATETIME NOT NULL,
    PRIMARY KEY (`id`),
    UNIQUE KEY `uq_characters_name` (`name`),
    KEY `idx_characters_account_id` (`account_id`)
);

-- Bootstrap first admin manually when needed:
-- UPDATE `accounts`
-- SET `admin_level` = 8, `updated_at` = NOW()
-- WHERE `username` = 'YourAccountName'
-- LIMIT 1;
