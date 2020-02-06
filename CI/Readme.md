Updating Build Information
==========================
Build information is updated in 1 of 2 ways, depending on which part of the version needs
to be modified. [Semantic Versioning terminology](https://semver.org) is used in the description
that follows.

    Example build number:                   <major>.<minor>.<patch>[-<prelease_build_name>][+<build info>]
    Examples:                               1.2.3-preview.1         ==  1.2.3-preview.1+with.build.info
                                                < 1.2.3-preview.2   <   1.2.3-preview.11
                                                < 1.2.3-zulu        <   1.2.3

`<major>`, `<minor>`, and `<patch>` build values are defined in cmake files and should be updated there;
for an example, see src/SharedLibrary/cmake/Featurizers.cmake.

`<prerelease_build_name>` is (optionally) defined when invoking the CI build. Additional information
(including steps to specify these values) follows.

Build Types
===========
The yaml build definitions in this directory are able to create difference build
types, each with different versioning, packaging, and publishing characteristics.
Build variables specified when invoking `CI.yaml` on Azure DevOps are used to
specific which build type is invoked.

All version numbers conform to [Semantic Versioning standards](https://semver.org)*.

Continuous Integration / Standard Build
---------------------------------------
The build type invoked by default (when no Azure DevOps variables are specified).

    Azure DevOps Variables:                 None
    Version:                                <major>.<minor>.<patch>-pipe.1.<year>.<month>.<day>.<hour>.<minute>.<second>
    Example:                                0.2.0-pipe.1.2019.10.23.8.47.19
    NuGet Store (internal):                 https://aiinfra.visualstudio.com/DataPipelines/_packaging
    NuGet Store (public):                   https://aiinfra.visualstudio.com/PublicPackages/_packaging?_a=feed&feed=FeaturizersLibrary

Prerelease
----------
An unofficial, but public, release.

    Azure DevOps Variables:                             prerelease_build_name=<build_name>
    Version:                                            <major>.<minor>.<patch>-<build_name>
    Example (where prerelease_build_name=preview.1):    0.2.0-preview.1
    NuGet Store:                                        https://www.nuget.org/

Release
-------
An official release.

    Azure DevOps Variables:                 release_build=True
    Version:                                <major>.<minor>.<patch>
    Example:                                0.2.0
    NuGet Store:                            https://www.nuget.org

\* Note that NuGet doesn't support SemVer's build info (e.g. "+\<build info here>") well, which is why
Continuous Integration datetime info is stored as prerelease info rather than build info.

Azure DevOps Configuration Settings
===================================

These Azure DevOps build definitions rely on the following Service Connections.
To add/remove/view Service Connections within Azure DevOps:

    1) Navigate to Azure DevOps Project Page
    2) Project Settings -> Service connections

AI Infra Build
--------------
Used to push binaries to Azure resources during publish.

    Type:                                               Azure Resource Manager
    Connection Name:                                    AI Infra Build (<guid>)
    Scope level:                                        Subscription
    Subscription:                                       AI Infra Build (<guid>)
    Resource Group:                                     FeaturizersLibraryBuild

ESRP CodeSigning Connection
---------------------------
Used to sign binaries and packages.

    Type:                                               ESRP CodeSigning
    Server URL:                                         https://api.esrp.microsoft.com/api/v2
    AAD APP Id:                                         <created in Azure>
    AAD APP Secret:                                     <created in Azure>
    PRSS Signing Authentication Certificate:            Created through the [ESRP Portal](https://portal.esrp.microsoft.com/)

Nuget
-----
Used to push release and prerelease NuGet packages.

    Type:                                               NuGet Service Connection
    Feed URL:                                           https://api.nuget.org/v3/index.json
    ApiKey:                                             Created on NuGet.org and then copied to the Azure DevOps connection dialog window for one-time usage

Nuget (Public CI Builds)
------------------------
Used to push CI, release, and prerelease NuGet packages.

    Type:                                   NuGet Service Connection
    Feed Url:                               https://aiinfra.pkgs.visualstudio.com/PublicPackages/_packaging/FeaturizersLibrary/nuget/v3/index.json
    ApiKey:                                 not_used (Note that this will not be used as the repository is in the same Azure DevOps project, but needs to have some value to continue in the UX)

featurizerslibrarybuild
-----------------------
Used to pull the universal_linux Docker image for universal_linux builds.

    Type:                                               Dockerregistry
    Registry Type:                                      Azure Container Registry
    Connection name:                                    featurizerslibrarybuild
    Azure subscription:                                 AI Infra Build
    Azure container registry:                           featurizerslibrarybuild

Azure DevOps Settings
=====================
The Publish build will tag sources upon successful completion. To do this, the Azure DevOps `Project Collection Build Service` account needs
contribute permissions to create tags for the appropriate repos. Follow the steps [here](https://docs.microsoft.com/en-us/azure/devops/pipelines/scripts/git-commands?view=azure-devops&tabs=yaml) to grant those permissions.
