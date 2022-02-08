-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
ALTER PROCEDURE [dbo].[AccountCheckPW]
	-- Add the parameters for the stored procedure here
	@id NCHAR(10),
	@pw NCHAR(10)
AS
BEGIN TRY
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;
	
    -- Insert statements for procedure here
	declare @ret NCHAR(10)
	SELECT @ret = user_pw from dbo.usertbl where @id = user_id
	if(@ret <> @pw)
		begin
		return 1;
		end
	execute dbo.AccountLoginUser
	return 0;
END TRY
BEGIN CATCH
	RETURN 2;

END CATCH