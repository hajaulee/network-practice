#include "schedule.c"

int main(int argc, char const *argv[])
{
    readScheduleFile("schedule.txt");
	puts(displaySchedule("Monday"));
    return 0;
}
