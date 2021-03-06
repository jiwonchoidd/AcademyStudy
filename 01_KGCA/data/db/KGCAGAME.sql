USE [master]
GO
/****** Object:  Database [KGCA3D]    Script Date: 10/29/2018 19:47:34 ******/
CREATE DATABASE [KGCA3D]
GO
ALTER DATABASE [KGCA3D] SET COMPATIBILITY_LEVEL = 100
GO
ALTER DATABASE [KGCA3D] SET ANSI_NULL_DEFAULT OFF
GO
ALTER DATABASE [KGCA3D] SET ANSI_NULLS OFF
GO
ALTER DATABASE [KGCA3D] SET ANSI_PADDING OFF
GO
ALTER DATABASE [KGCA3D] SET ANSI_WARNINGS OFF
GO
ALTER DATABASE [KGCA3D] SET ARITHABORT OFF
GO
ALTER DATABASE [KGCA3D] SET AUTO_CLOSE OFF
GO
ALTER DATABASE [KGCA3D] SET AUTO_CREATE_STATISTICS ON
GO
ALTER DATABASE [KGCA3D] SET AUTO_SHRINK OFF
GO
ALTER DATABASE [KGCA3D] SET AUTO_UPDATE_STATISTICS ON
GO
ALTER DATABASE [KGCA3D] SET CURSOR_CLOSE_ON_COMMIT OFF
GO
ALTER DATABASE [KGCA3D] SET CURSOR_DEFAULT  GLOBAL
GO
ALTER DATABASE [KGCA3D] SET CONCAT_NULL_YIELDS_NULL OFF
GO
ALTER DATABASE [KGCA3D] SET NUMERIC_ROUNDABORT OFF
GO
ALTER DATABASE [KGCA3D] SET QUOTED_IDENTIFIER OFF
GO
ALTER DATABASE [KGCA3D] SET RECURSIVE_TRIGGERS OFF
GO
ALTER DATABASE [KGCA3D] SET  DISABLE_BROKER
GO
ALTER DATABASE [KGCA3D] SET AUTO_UPDATE_STATISTICS_ASYNC OFF
GO
ALTER DATABASE [KGCA3D] SET DATE_CORRELATION_OPTIMIZATION OFF
GO
ALTER DATABASE [KGCA3D] SET TRUSTWORTHY OFF
GO
ALTER DATABASE [KGCA3D] SET ALLOW_SNAPSHOT_ISOLATION OFF
GO
ALTER DATABASE [KGCA3D] SET PARAMETERIZATION SIMPLE
GO
ALTER DATABASE [KGCA3D] SET READ_COMMITTED_SNAPSHOT OFF
GO
ALTER DATABASE [KGCA3D] SET HONOR_BROKER_PRIORITY OFF
GO
ALTER DATABASE [KGCA3D] SET  READ_WRITE
GO
ALTER DATABASE [KGCA3D] SET RECOVERY FULL
GO
ALTER DATABASE [KGCA3D] SET  MULTI_USER
GO
ALTER DATABASE [KGCA3D] SET PAGE_VERIFY CHECKSUM
GO
ALTER DATABASE [KGCA3D] SET DB_CHAINING OFF
GO
USE [KGCA3D]
GO
/****** Object:  Table [dbo].[USERLIST]    Script Date: 10/29/2018 19:47:36 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[USERLIST](
	[USERID] [nchar](10) NULL,
	[USERPS] [nchar](10) NULL
) ON [PRIMARY]
GO
INSERT [dbo].[USERLIST] ([USERID], [USERPS]) VALUES (N'AAA       ', N'BBBB      ')
INSERT [dbo].[USERLIST] ([USERID], [USERPS]) VALUES (N'홍길동       ', N'오뚜기       ')
INSERT [dbo].[USERLIST] ([USERID], [USERPS]) VALUES (N'DFDSAF    ', N'FDFD      ')
/****** Object:  StoredProcedure [dbo].[Find_ByID]    Script Date: 10/29/2018 19:47:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[Find_ByID] (@pPersonID int, @pPersonIX int) AS SELECT PersonID, FirstName, LastName, Address, City FROM TestTBL1 WHERE PersonID=@pPersonID; RETURN @pPersonIX
GO
/****** Object:  Table [dbo].[Account]    Script Date: 10/29/2018 19:47:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[Account](
	[USERID] [nchar](10) NOT NULL,
	[USERPS] [nchar](10) NOT NULL,
	[ACCOUNTTIME] [smalldatetime] NOT NULL,
	[Login] [smalldatetime] NULL,
	[Logout] [smalldatetime] NULL,
 CONSTRAINT [PK_Account] PRIMARY KEY CLUSTERED 
(
	[USERID] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]
) ON [PRIMARY]
GO
INSERT [dbo].[Account] ([USERID], [USERPS], [ACCOUNTTIME], [Login], [Logout]) VALUES (N'거북이       ', N'DDDD      ', CAST(0xA988044B AS SmallDateTime), CAST(0xA988044B AS SmallDateTime), NULL)
INSERT [dbo].[Account] ([USERID], [USERPS], [ACCOUNTTIME], [Login], [Logout]) VALUES (N'홍길동       ', N'gggg      ', CAST(0xA9880440 AS SmallDateTime), CAST(0xA9880443 AS SmallDateTime), CAST(0xA9880444 AS SmallDateTime))
INSERT [dbo].[Account] ([USERID], [USERPS], [ACCOUNTTIME], [Login], [Logout]) VALUES (N'gggg      ', N'dfdfd     ', CAST(0xA9880442 AS SmallDateTime), NULL, NULL)
/****** Object:  StoredProcedure [dbo].[usp_userinfo]    Script Date: 10/29/2018 19:47:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[usp_userinfo]	
	@UID NCHAR(10)
	
AS
BEGIN
	SELECT * FROM dbo.Account
	WHERE @UID=USERID;
END
RETURN 3;
GO
/****** Object:  StoredProcedure [dbo].[usp_Test]    Script Date: 10/29/2018 19:47:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[usp_Test]
AS
	SELECT * FROM dbo.USERLIST;
GO
/****** Object:  StoredProcedure [dbo].[usp_passout]    Script Date: 10/29/2018 19:47:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[usp_passout]	
	@UID NCHAR(10),
	@UPS NCHAR(10) OUTPUT	
AS
BEGIN
	SELECT @UPS=USERPS FROM dbo.USERLIST
	WHERE @UID=USERID;
END
RETURN 1;
GO
/****** Object:  StoredProcedure [dbo].[usp_pass]    Script Date: 10/29/2018 19:47:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[usp_pass]	
	@UID NCHAR(10)
	
AS
BEGIN
	SELECT * FROM dbo.USERLIST
	WHERE @UID=USERID;
END
RETURN 3;
GO
/****** Object:  StoredProcedure [dbo].[LogoutUser]    Script Date: 10/29/2018 19:47:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[LogoutUser]	
	@UID NCHAR(10),
	@UPS NCHAR(10)
AS
UPDATE KGCA3D.dbo.Account
	SET Logout = GETDATE()
	WHERE USERID = @UID
RETURN 3;
GO
/****** Object:  StoredProcedure [dbo].[LoginUser]    Script Date: 10/29/2018 19:47:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[LoginUser]	
	@UID NCHAR(10),
	@UPS NCHAR(10)
AS
UPDATE KGCA3D.dbo.Account
	SET Login = GETDATE()
	WHERE USERID = @UID
RETURN 3;
GO
/****** Object:  StoredProcedure [dbo].[AccountCreate]    Script Date: 10/29/2018 19:47:37 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- 계정 생성 성공 : 0
-- 중복 경우 : 1
-- 이외 경우 : 2
-- =============================================
CREATE PROCEDURE [dbo].[AccountCreate]	
	@UID NCHAR(10),
	@UPS NCHAR(10)
AS
	SET NOCOUNT ON
	BEGIN TRY
		BEGIN TRAN
			INSERT INTO Account(USERID, USERPS, ACCOUNTTIME)
			VALUES( @UID, @UPS, default );

			EXECUTE KGCA3D.dbo.LoginUser @UID, @UPS
		COMMIT TRAN
		RETURN 0;
	END TRY

	BEGIN CATCH
		ROLLBACK
		DECLARE @ret NCHAR(10)
		SELECT @ret = USERID
		FROM Account
		WHERE USERID = @UID
		-- 이미 존재하는 ID-> 1
		IF(@ret <>  N'')
			BEGIN
				RETURN 1;
			END
		RETURN 2;
	END CATCH
GO
/****** Object:  Default [DF__Table__ACCOUNTTI__09DE7BCC]    Script Date: 10/29/2018 19:47:37 ******/
ALTER TABLE [dbo].[Account] ADD  DEFAULT (getdate()) FOR [ACCOUNTTIME]
GO
