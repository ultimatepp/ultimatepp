void Inc(int& a)
{
	a++;
}

int x;

void Dec(int& a)
{
	if(--a == 0) x++;
}
