CREATE TABLE IF NOT EXISTS `accounts` (
    `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
    `username` VARCHAR(24) NOT NULL,
    `password_hash` VARCHAR(128) NOT NULL,
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

CREATE TABLE IF NOT EXISTS `admin_accounts` (
    `account_id` INT UNSIGNED NOT NULL,
    `level` TINYINT UNSIGNED NOT NULL DEFAULT 0,
    `assigned_by` INT UNSIGNED NULL DEFAULT NULL,
    `assigned_at` DATETIME NULL DEFAULT NULL,
    `updated_at` DATETIME NULL DEFAULT NULL,
    PRIMARY KEY (`account_id`),
    KEY `idx_admin_accounts_level` (`level`)
);

-- Bootstrap first admin manually when needed:
-- INSERT INTO `admin_accounts` (`account_id`, `level`, `assigned_by`, `assigned_at`, `updated_at`)
-- SELECT `id`, 8, NULL, NOW(), NOW()
-- FROM `accounts`
-- WHERE `username` = 'YourAccountName'
-- ON DUPLICATE KEY UPDATE
--     `level` = VALUES(`level`),
--     `assigned_by` = VALUES(`assigned_by`),
--     `assigned_at` = NOW(),
--     `updated_at` = NOW();
