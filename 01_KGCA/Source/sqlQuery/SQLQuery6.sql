declare @a int;
declare @b int;
set @a = 1;
set @b = 2;
select @a as 'A 스왑 전', @b as 'B 스왑 전';
exec swap_int @a output , @b output
select @a as 'A 스왑 후', @b as 'B 스왑 후';
go