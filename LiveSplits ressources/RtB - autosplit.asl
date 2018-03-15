state("RollPlayingGameUE4-Win64-Shipping")
{
	int levelNumPtr : 0x02025E48, 0x1E0, 0x138, 0x7C; //Adress pointer
}

split
{
	if (current.levelNumPtr != old.levelNumPtr)
	{
		return (true);
	}
}