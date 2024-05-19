from os.path import join
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import PythonExpression

def generate_launch_description():
    world_package = get_package_share_directory('msr_world')

    use_sim_time = LaunchConfiguration('use_sim_time', default='true')

    gazebo_share = get_package_share_directory("gazebo_ros")
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(join(gazebo_share, "launch", "gazebo.launch.py")),
        launch_arguments={"verbose": "true"}.items(),
    )

    return LaunchDescription([
        DeclareLaunchArgument('world', default_value=[PythonExpression(['"',join(world_package, 'worlds'),'" + "/floor.world"']),'']),
        DeclareLaunchArgument('gui', default_value='true'),
        DeclareLaunchArgument('verbose', default_value='true'),
        DeclareLaunchArgument('use_sim_time', default_value = use_sim_time),
        gazebo,
    ])