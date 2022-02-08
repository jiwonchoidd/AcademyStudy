ALTER PROCEDURE [dbo].[AccountCreate]
	@id NCHAR(10),
	@pw NCHAR(10)
	AS
	--실행 결과 출력하지 않는다.
	SET NOCOUNT ON 
	BEGIN TRY
		--중복처리가 되고 있었다.
		declare @ret NCHAR(10)
		select @ret = @id from usertbl where user_id = @id
		
		--아이디가 이미 존재하면 리턴 
		--리턴값이 N'' 널값일때 찾은 값이 없으므로 리턴 2
		if(@ret<>N'')
		BEGIN
			RETURN 1;
		END
	ELSE
		BEGIN
			INSERT INTO usertbl(user_id, user_pw,account_time)
			VALUES(@id, @pw, default);
		END
		RETURN 0;
	END TRY

	BEGIN CATCH
	 --예외사항
		RETURN 2;
	END CATCH